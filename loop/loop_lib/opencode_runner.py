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
#   File:   loop/loop_lib/opencode_runner.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-21 18:50:00
#   


import os
import json

from loop.loop_lib.process_guard import run_guarded, guarded_result


_BASH_TIMEOUT_MS = "120000"


def _build_command(
     prompt:            str
    ,target_root:       str
    ,agent:             str | None
    ,model:             str | None
    ,session_id:        str | None
    ,continue_session:  bool
    ,title:             str | None
    ,flg_json_format:   bool
    ) -> list[ str ]:
    cmd = [ "opencode", "run", "--auto" ]
    if flg_json_format:
        cmd += [ "--format", "json" ]
    if agent:
        cmd += [ "--agent", agent ]
    if model:
        cmd += [ "--model", model ]
    if session_id:
        cmd += [ "--session", session_id ]
    elif continue_session:
        cmd += [ "--continue" ]
    if title:
        cmd += [ "--title", title ]
    cmd += [ "--dir", target_root ]
    cmd.append( prompt )
    return  cmd


def _build_env( ) -> dict:
    return  os.environ | {
        "OPENCODE_EXPERIMENTAL_BASH_DEFAULT_TIMEOUT_MS": _BASH_TIMEOUT_MS,
    }


def run_opencode(
     prompt:            str
    ,target_root:       str
    ,agent:             str | None      = None
    ,model:             str | None      = None
    ,session_id:        str | None      = None
    ,continue_session:  bool            = False
    ,title:             str | None      = None
    ,timeout:           int             = 600
    ,flg_json_format:   bool            = True
    ) -> guarded_result:
    """runs opencode run --auto with hangout protection
    returns a guarded_result with stdout stderr and timeout status"""
    cmd = _build_command(
         prompt           = prompt
        ,target_root      = target_root
        ,agent            = agent
        ,model            = model
        ,session_id       = session_id
        ,continue_session = continue_session
        ,title            = title
        ,flg_json_format  = flg_json_format
    )
    env = _build_env( )
    return  run_guarded( cmd, timeout = timeout, env = env, cwd = target_root )


def extract_session_id( json_output: str ) -> str | None:
    """extracts the session id from opencode --format json ndjson output
    each line is a json event with a session id field
    returns none if no session id is found"""
    for line in json_output.strip( ).split( "\n" ):
        line = line.strip( )
        if not line:
            continue
        try:
            event = json.loads( line )
        except json.JSONDecodeError:
            continue
        sid = _find_session_id( event )
        if sid:
            return  sid
    return  None


def _find_session_id( event: dict ) -> str | None:
    for key in ( "sessionID", "sessionId", "session_id" ):
        value = event.get( key )
        if isinstance( value, str ) and value:
            return  value
    info = event.get( "info" )
    if isinstance( info, dict ):
        sid = info.get( "id" )
        if isinstance( sid, str ) and sid:
            return  sid
    session = event.get( "session" )
    if isinstance( session, dict ):
        sid = session.get( "id" )
        if isinstance( sid, str ) and sid:
            return  sid
    return  None


def extract_text_output( json_output: str ) -> str:
    """extracts human-readable text from opencode --format json ndjson output
    concatenates all text parts from message events
    falls back to the raw output if no text parts are found"""
    text_parts = [ ]
    for line in json_output.strip( ).split( "\n" ):
        line = line.strip( )
        if not line:
            continue
        try:
            event = json.loads( line )
        except json.JSONDecodeError:
            continue
        text = _extract_text_from_event( event )
        if text:
            text_parts.append( text )
    if text_parts:
        return  "\n".join( text_parts )
    return  json_output


def _extract_text_from_event( event: dict ) -> str | None:
    part = event.get( "part" )
    if isinstance( part, dict ):
        if part.get( "type" ) == "text":
            text = part.get( "text" )
            if isinstance( text, str ):
                return  text
    info = event.get( "info" )
    if isinstance( info, dict ):
        if info.get( "type" ) == "text":
            text = info.get( "text" )
            if isinstance( text, str ):
                return  text
    return  None
