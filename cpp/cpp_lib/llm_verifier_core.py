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
#   File:   cpp/cpp_lib/llm_verifier_core.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-19 00:00:00
#   


#

import os
import re
from datetime import datetime
from dataclasses import dataclass
from concurrent.futures import ThreadPoolExecutor, as_completed

from sak.common import (
     ensure
    ,ensure_list
    ,to_json
    ,create_process
    ,get_process_text
    ,value_or
)
from sak.fso.get_file_list import get_file_list
from cpp.cpp_lib.llm_rules import (
     llm_rule
    ,get_all_rules
    ,get_rules_by_ids
    ,build_verification_prompt
    ,build_verification_prompt_all_rules
)


_parallel_workers = 8


@dataclass
class llm_violation:
    file_path: str
    rule_id: str
    rule_name: str
    line_number: int
    line_content: str
    description: str

    @property
    def to_dict( self ):
        return  {
             "file_path": self.file_path
            ,"rule_id": self.rule_id
            ,"rule_name": self.rule_name
            ,"line_number": self.line_number
            ,"line_content": self.line_content
            ,"description": self.description
        }

    def __repr__( self ):
        return  to_json( self.to_dict )


@dataclass
class llm_verifier_result:
    total_checks: int
    checked_files: list[ str ]
    checked_rules: list[ str ]
    potential_violations: list[ llm_violation ]
    skipped_files: list[ str ]

    @property
    def to_dict( self ):
        return  {
             "total_checks": self.total_checks
            ,"checked_files": self.checked_files
            ,"checked_rules": self.checked_rules
            ,"potential_violations": [ v.to_dict for v in self.potential_violations ]
            ,"skipped_files": self.skipped_files
        }

    def __repr__( self ):
        return  to_json( self.to_dict )


def _read_file( file_path: str ) -> str | None:
    try:
        with open( file_path, "r", encoding = "utf-8" ) as f:
            return  f.read( )
    except ( FileNotFoundError, PermissionError, UnicodeDecodeError ):
        return  None


def _strip_preamble( raw_text: str ) -> str:
    return  "\n".join( [ line for line in raw_text.split( "\n" ) if ( stripped := line.strip( ) ) and not stripped.startswith( "> " ) ] )


def _parse_violation( raw_text: str, rules_by_id: dict[ str, llm_rule ], file_path: str, file_lines: list[ str ] ) -> list[ llm_violation ]:
    text = _strip_preamble( raw_text )
    if text.strip( ).lower( ) == "ok":
        return  [ ]

    violations = [ ]
    pattern = r"violation:\s*(\w+)\s*line\s*(\d+)\s*:\s*(.+?)(?=\n\s*violation|\Z)"
    matches = re.findall( pattern, text, re.IGNORECASE | re.DOTALL )
    for match in matches:
        rule_id    = match[ 0 ]
        line_number = int( match[ 1 ] )
        description = match[ 2 ].strip( )

        rule = rules_by_id.get( rule_id )
        rule_name = rule.name if rule else rule_id

        line_content = ""
        if 1 <= line_number <= len( file_lines ):
            line_content = file_lines[ line_number - 1 ].strip( )
        violations.append( llm_violation(
             file_path    = file_path
            ,rule_id      = rule_id
            ,rule_name    = rule_name
            ,line_number  = line_number
            ,line_content = line_content
            ,description  = description
        ) )
    return  violations


def _check_file_all_rules( file_path: str, rules: list[ llm_rule ], target_root: str, model: str ) -> list[ llm_violation ]:
    absolute_path = os.path.join( target_root, file_path ) if not os.path.isabs( file_path ) else file_path
    content = _read_file( absolute_path )
    if content is None:
        return  [ ]

    file_lines = content.split( "\n" )
    try:
        return  _parse_violation(
             get_process_text( create_process( [
                  "opencode", "run", "--auto"
                 ,"--model", model
                 ,"--dir", target_root
                 ,build_verification_prompt_all_rules( rules, file_path, content )
             ] ) )
            ,{ r.rule_id: r for r in rules }
            ,file_path
            ,file_lines
        )
    except Exception:
        return  [ ]


def _get_files_to_check( files: list[ str ] | None, target_root: str ) -> list[ str ]:
    if files is None:
        files = get_file_list( target_root, extensions = [ ".cpp", ".hpp" ], flg_recursive = True )
    return  ensure_list( files, str )


def _get_rules_to_check( rule_ids: list[ str ] | None ) -> list[ llm_rule ]:
    return  get_all_rules( ) if rule_ids is None else get_rules_by_ids( rule_ids )


def run_llm_verifier( files: list[ str ] | None, rule_ids: list[ str ] | None, target_root: str, model: str ) -> llm_verifier_result:
    ensure( os.path.isdir( target_root ), f"target root is not a valid directory: {target_root}" )
    ensure( model, "model must not be empty" )

    checked_files = _get_files_to_check( files, target_root )
    checked_rules = _get_rules_to_check( rule_ids )

    total_checks = len( checked_files ) * len( checked_rules )
    violations: list[ llm_violation ] = [ ]
    skipped: list[ str ] = [ ]

    if total_checks == 0:
        return  llm_verifier_result(
             total_checks         = 0
            ,checked_files        = checked_files
            ,checked_rules        = [ r.rule_id for r in checked_rules ]
            ,potential_violations = [ ]
            ,skipped_files        = [ ]
        )

    with ThreadPoolExecutor( max_workers = _parallel_workers ) as executor:
        futures = {
             executor.submit( _check_file_all_rules, file_path, checked_rules, target_root, model ): file_path
            for file_path in checked_files
        }
        for future in as_completed( futures ):
            file_path = futures[ future ]
            try:
                result = future.result( )
                violations.extend( result )
            except Exception as e:
                skipped.append( f"{file_path} x all rules: {e}" )

    return  llm_verifier_result(
         total_checks         = total_checks
        ,checked_files        = checked_files
        ,checked_rules        = [ r.rule_id for r in checked_rules ]
        ,potential_violations = violations
        ,skipped_files        = skipped
    )


def write_markdown_report( result: llm_verifier_result, output_path: str ) -> str:
    now_str = datetime.now( ).strftime( "%Y-%m-%d %H:%M:%S" )
    report = f"# cpp style violations\n\n"
    report += f"verified on: { now_str }\n"
    report += f"files checked: { len( result.checked_files ) }\n"
    report += f"rules checked: { len( result.checked_rules ) }\n\n"

    violations_by_rule: dict[ str, list[ llm_violation ] ] = { }
    for v in result.potential_violations:
        key = v.rule_name + " (" + v.rule_id + ")"
        violations_by_rule.setdefault( key, [ ] ).append( v )

    for rule_key, violist in violations_by_rule.items( ):
        report += f"## {rule_key}\n\n"
        for v in violist:
            display_path = f"{v.file_path}:{v.line_number}"
            report += f"### {display_path}\n\n"
            if v.line_content:
                report += f"```\n{v.line_content}\n```\n\n"
            report += f"**issue:** {v.description}\n\n"
            report += f"**fix:**\n\n"

    os.makedirs( os.path.dirname( os.path.abspath( output_path ) ), exist_ok = True )
    with open( output_path, "w", encoding = "utf-8" ) as f:
        f.write( report )
    return  output_path
