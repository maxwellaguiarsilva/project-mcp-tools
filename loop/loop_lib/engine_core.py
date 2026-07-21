#!/usr/bin/python3

#   
#   Copyright (C) 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#   
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#   
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#   
#   
#   File:   loop/loop_lib/engine_core.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-21 19:05:00
#   


import os
import time
import json
from dataclasses import dataclass, field

from sak.common import to_json, to_dict, ensure

from loop.loop_lib.process_guard import guarded_result
from loop.loop_lib.opencode_runner import run_opencode, extract_session_id, extract_text_output
from loop.loop_lib.verdict import parse_verdict, verdict
from loop.loop_lib.session_monitor import query_session, query_latest_session


@dataclass
class loop_config:
    target_root: str
    task: str = ""
    spec_dir: str = "docs/agent"
    max_iterations: int = 20
    programmer_timeout: int = 600
    police_timeout: int = 300
    programmer_model: str | None = None
    police_model: str | None = None
    log_file: str = "loop-engine.log"
    flg_dry_run: bool = False

    @property
    def to_dict( self ):
        return  to_dict( self, [
            "target_root", "task", "spec_dir", "max_iterations",
            "programmer_timeout", "police_timeout",
            "programmer_model", "police_model",
            "log_file", "flg_dry_run",
        ] )


@dataclass
class loop_result:
    success: bool
    iterations: int
    final_verdict: verdict | None = None
    log_path: str = ""

    @property
    def to_dict( self ):
        return  {
             "success":        self.success
            ,"iterations":     self.iterations
            ,"final_verdict":  self.final_verdict.to_dict if self.final_verdict else None
            ,"log_path":       self.log_path
        }


def run_loop( config: loop_config ) -> loop_result:
    """main loop: programmer implements then police evaluates
    if police is not satisfied feedback is fed back to the programmer
    loops until satisfied or max_iterations is reached
    every subprocess is protected by a hard timeout to prevent hangouts"""
    ensure( os.path.isdir( config.target_root ), f"target root not found: {config.target_root}" )
    _ensure_task( config )
    _log( config, { "event": "loop_start", "config": config.to_dict } )
    programmer_session_id: str | None = None
    feedback = ""
    last_verdict: verdict | None = None
    for iteration in range( 1, config.max_iterations + 1 ):
        _log( config, { "event": "iteration_start", "iteration": iteration } )
        prog_result = _run_programmer( config, iteration, programmer_session_id, feedback )
        if prog_result is None:
            feedback = "previous programmer iteration timed out (hangout) check for infinite loops or blocking operations and avoid them"
            continue
        if programmer_session_id is None:
            programmer_session_id = _extract_prog_session_id( prog_result )
        police_result = _run_police( config, iteration )
        if police_result is None:
            feedback = "police evaluation timed out verify your changes are complete"
            continue
        police_text = extract_text_output( police_result.stdout )
        v = parse_verdict( police_text )
        last_verdict = v
        _log( config, {
            "event":      "verdict",
            "iteration":  iteration,
            "satisfied":  v.satisfied,
            "score":      v.score,
            "issues":     len( v.issues ),
            "parse_error": v.parse_error,
        } )
        if v.satisfied:
            _log( config, { "event": "loop_success", "iteration": iteration } )
            _print_success( iteration, v )
            return  loop_result(
                 success       = True
                ,iterations    = iteration
                ,final_verdict = v
                ,log_path      = config.log_file
            )
        feedback = v.feedback or v.summary or "police was not satisfied see issues for details"
        _print_iteration_summary( iteration, v )
    _log( config, { "event": "loop_max_iterations", "max": config.max_iterations } )
    _print_max_iterations( config.max_iterations, last_verdict )
    return  loop_result(
         success       = False
        ,iterations    = config.max_iterations
        ,final_verdict = last_verdict
        ,log_path      = config.log_file
    )


def _ensure_task( config: loop_config ) -> None:
    if config.task:
        return
    config.task = _build_default_task( config )


def _build_default_task( config: loop_config ) -> str:
    spec_path = os.path.join( config.target_root, config.spec_dir )
    return  f"""implement the super fruit world game according to the specification
read all files in {spec_path}
start with the requirements checklist in requirements.md
work through each section systematically
verify your work against the compliance rules in compliance-rules.md"""


def _run_programmer( config: loop_config, iteration: int, session_id: str | None, feedback: str ) -> guarded_result | None:
    prompt = _build_programmer_prompt( config, feedback, iteration )
    if config.flg_dry_run:
        _print_dry_run( "programmer", prompt, config.programmer_timeout )
        return  None
    _print_phase( iteration, "programmer", "running" )
    result = run_opencode(
         prompt            = prompt
        ,target_root       = config.target_root
        ,agent             = "build"
        ,model             = config.programmer_model
        ,session_id        = session_id
        ,continue_session  = session_id is not None
        ,title             = f"loop-programmer-{iteration}"
        ,timeout           = config.programmer_timeout
        ,flg_json_format   = True
    )
    _log( config, {
         "event":      "phase_complete"
        ,"iteration":  iteration
        ,"phase":      "programmer"
        ,"success":    result.success
        ,"timed_out":  result.timed_out
        ,"duration_s": result.duration_s
    } )
    if result.timed_out:
        _print_timeout( iteration, "programmer", config.programmer_timeout )
        return  None
    _print_phase( iteration, "programmer", "done" )
    return  result


def _build_programmer_prompt( config: loop_config, feedback: str, iteration: int ) -> str:
    parts = [ config.task ]
    if iteration > 1 and feedback:
        parts.append( f"\n\nfeedback from the police agent (iteration {iteration - 1}):\n{feedback}" )
        parts.append( "\naddress every issue the police identified. do not skip any." )
    return  "\n".join( parts )


def _extract_prog_session_id( result ) -> str | None:
    sid = extract_session_id( result.stdout )
    if sid:
        return  sid
    latest = query_latest_session( )
    if latest and latest.found:
        return  latest.session_id
    return  None


def _run_police( config: loop_config, iteration: int ) -> guarded_result | None:
    prompt = _build_police_prompt( config )
    if config.flg_dry_run:
        _print_dry_run( "police", prompt, config.police_timeout )
        return  None
    _print_phase( iteration, "police", "running" )
    result = run_opencode(
         prompt            = prompt
        ,target_root       = config.target_root
        ,agent             = "police"
        ,model             = config.police_model
        ,session_id        = None
        ,continue_session  = False
        ,title             = f"loop-police-{iteration}"
        ,timeout           = config.police_timeout
        ,flg_json_format   = True
    )
    _log( config, {
         "event":      "phase_complete"
        ,"iteration":  iteration
        ,"phase":      "police"
        ,"success":    result.success
        ,"timed_out":  result.timed_out
        ,"duration_s": result.duration_s
    } )
    if result.timed_out:
        _print_timeout( iteration, "police", config.police_timeout )
        return  None
    _print_phase( iteration, "police", "done" )
    return  result


def _build_police_prompt( config: loop_config ) -> str:
    spec_path = os.path.join( config.target_root, config.spec_dir )
    return  f"""evaluate the current state of the super fruit world game against the specification

specification directory: {spec_path}
key files to read:
  - {spec_path}/requirements.md (checklist of all required features)
  - {spec_path}/game-design.md (detailed game design)
  - {spec_path}/compliance-rules.md (non-negotiable code quality rules)
  - {spec_path}/technical-architecture.md (architecture spec)
  - {spec_path}/identity-rules.md (visual identity rules)

steps:
1. read the specification files listed above
2. inspect the current codebase under src/ and data/
3. run git diff to see what has been implemented so far
4. use playwright to launch the dev server (bun run dev on port 3000) and verify the ui
5. check compliance rules against the actual code
6. return your verdict as a json block with this exact structure:

{{
  "satisfied": false,
  "score": 0,
  "summary": "brief overall assessment",
  "issues": [
    {{
      "severity": "critical",
      "category": "specification",
      "description": "what is wrong",
      "file": "path/to/file.js",
      "spec_reference": "rule id or section reference"
    }}
  ],
  "feedback": "actionable feedback for the programmer"
}}

set satisfied to true only if every requirement is met and no compliance violations exist
be rigorous and thorough"""


def _log( config: loop_config, entry: dict ) -> None:
    entry = { "timestamp": int( time.time( ) ), **entry }
    log_path = os.path.join( config.target_root, config.log_file )
    os.makedirs( os.path.dirname( os.path.abspath( log_path ) ), exist_ok = True )
    with open( log_path, "a", encoding = "utf-8" ) as f:
        f.write( json.dumps( entry, default = str ) + "\n" )


def _print_phase( iteration: int, phase: str, status: str ) -> None:
    print( f"[loop] iteration {iteration} | {phase} | {status}" )


def _print_timeout( iteration: int, phase: str, timeout: int ) -> None:
    print( f"[loop] iteration {iteration} | {phase} | TIMEOUT after {timeout}s (hangout detected)" )


def _print_dry_run( phase: str, prompt: str, timeout: int ) -> None:
    print( f"[loop] dry-run | {phase} | timeout={timeout}s" )
    print( f"  prompt: {prompt[ :200 ]}..." )


def _print_iteration_summary( iteration: int, v: verdict ) -> None:
    print( f"[loop] iteration {iteration} | verdict: NOT satisfied | score: {v.score} | issues: {len( v.issues )}" )
    if v.parse_error:
        print( f"[loop] warning: police verdict could not be parsed" )


def _print_success( iteration: int, v: verdict ) -> None:
    print( f"[loop] iteration {iteration} | verdict: SATISFIED | score: {v.score}" )
    print( f"[loop] summary: {v.summary}" )


def _print_max_iterations( max_iterations: int, v: verdict | None ) -> None:
    print( f"[loop] max iterations ({max_iterations}) reached without satisfaction" )
    if v:
        print( f"[loop] last score: {v.score} | last issues: {len( v.issues )}" )
