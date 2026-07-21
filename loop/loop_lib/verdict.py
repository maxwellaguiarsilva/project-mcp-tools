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
#   File:   loop/loop_lib/verdict.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-21 18:55:00
#   


import json
from dataclasses import dataclass, field

from sak.common import to_json


@dataclass
class verdict_issue:
    severity: str = ""
    category: str = ""
    description: str = ""
    file: str = ""
    spec_reference: str = ""

    @property
    def to_dict( self ):
        return  to_dict( self, [ "severity", "category", "description", "file", "spec_reference" ] )


@dataclass
class verdict:
    satisfied: bool = False
    score: int = 0
    summary: str = ""
    issues: list[ verdict_issue ] = field( default_factory = list )
    feedback: str = ""
    parse_error: bool = False

    @property
    def to_dict( self ):
        return  {
             "satisfied":   self.satisfied
            ,"score":       self.score
            ,"summary":     self.summary
            ,"issues":      [ issue.to_dict for issue in self.issues ]
            ,"feedback":    self.feedback
            ,"parse_error": self.parse_error
        }

    def __repr__( self ):
        return  to_json( self.to_dict )


def parse_verdict( raw_output: str ) -> verdict:
    """extracts the json verdict from the police agent output
    the police agent is instructed to output a json block containing
    satisfied score summary issues and feedback
    returns a verdict with parse_error true if extraction fails"""
    json_blocks = _extract_json_blocks( raw_output )
    for block in reversed( json_blocks ):
        v = _try_parse_verdict( block )
        if v is not None:
            return  v
    return  verdict(
         satisfied   = False
        ,feedback    = "failed to parse police verdict from output"
        ,parse_error = True
    )


def _extract_json_blocks( text: str ) -> list[ str ]:
    """extracts all top-level json blocks from text by tracking brace depth
    handles nested braces correctly"""
    blocks = [ ]
    i = 0
    while i < len( text ):
        if text[ i ] != "{":
            i += 1
            continue
        depth = 0
        for j in range( i, len( text ) ):
            if text[ j ] == "{":
                depth += 1
            elif text[ j ] == "}":
                depth -= 1
                if depth == 0:
                    blocks.append( text[ i : j + 1 ] )
                    i = j + 1
                    break
        else:
            break
    return  blocks


def _try_parse_verdict( block: str ) -> verdict | None:
    try:
        data = json.loads( block )
    except json.JSONDecodeError:
        return  None
    if not isinstance( data, dict ):
        return  None
    if "satisfied" not in data:
        return  None
    issues = [
        verdict_issue(
             severity      = issue.get( "severity", "" )
            ,category      = issue.get( "category", "" )
            ,description   = issue.get( "description", "" )
            ,file          = issue.get( "file", "" )
            ,spec_reference = issue.get( "spec_reference", "" )
        )
        for issue in data.get( "issues", [ ] )
        if isinstance( issue, dict )
    ]
    return  verdict(
         satisfied   = bool( data.get( "satisfied", False ) )
        ,score       = int( data.get( "score", 0 ) )
        ,summary     = str( data.get( "summary", "" ) )
        ,issues      = issues
        ,feedback    = str( data.get( "feedback", "" ) )
        ,parse_error = False
    )
