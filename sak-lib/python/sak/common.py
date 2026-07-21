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
#   File:   sak/common.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-01-13 19:46:34
#   


#


#   #ignore-helpers-rule


import json
import sys
import os
import re
import subprocess
from datetime import datetime


def print_line( strong: bool = True ) -> None:
    print( ( "=" if strong else "-" ) * 50 )


def value_or( values, index, default ):
    try:
        return  values[ index ]
    except ( IndexError, KeyError ):
        return  default


def if_in( value, valid_list, default = None ):
    return  value if value in valid_list else default


def ensure( expression: bool, message: str ):
    if not expression:
        raise   Exception( message )


def all_is_instance( items, cls: type ) -> bool:
    return  isinstance( items, cls ) or (
        isinstance( items, list ) and all( isinstance( item, cls ) for item in items )
    )


def remove_string_prefix( text: str, prefix: str ):
    return  text[ len( prefix ): ] if text.startswith( prefix ) else text


def ensure_list( value, cls: type | None = None, message: str = "" ) -> list:
    ensure(
         cls is None or all_is_instance( value, cls )
        ,message or f"invalid parameter: must be a value or a list of values"
    )
    return  value if isinstance( value, list ) else [ value ]


def datetime_from_string( format: str, value: str ) -> datetime | None:
    try:
        return  datetime.strptime( value, format )
    except ( ValueError, TypeError ):
        return  None


def extract_string( pattern: str, text: str ) -> list[ str ]:
    match = re.search( pattern, text )
    return  list( match.groups( ) ) if match else [ ]


def deep_update( source: dict, overrides: dict ) -> dict:
    return  {
        key: deep_update( source[ key ], overrides[ key ] )
        if key in source and isinstance( source[ key ], dict ) and isinstance( overrides.get( key ), dict )
        else overrides.get( key, source.get( key ) )
        for key in source.keys( ) | overrides.keys( )
    }


def to_dict( obj, members: list[ str ] ) -> dict:
    def __to_dict( value ):
        if hasattr( value, "to_dict" ):
            return  value.to_dict
        if isinstance( value, dict ):
            return  { key: __to_dict( val ) for key, val in value.items( ) }
        if isinstance( value, list ):
            return  [ __to_dict( val ) for val in value ]
        return  value

    return  { member: __to_dict( getattr( obj, member ) ) for member in members }


def to_json( data, flg_indent: bool = True ) -> str:
    return  json.dumps( data, indent = 4 if flg_indent else None, default = str )


def from_json( text: str ) -> dict | list:
    return  json.loads( text ) if text else { }


def create_process( command: list[ str ] | str, **kwargs ):
    params = { "check": True, "capture_output": True, "text": True }
    params.update( kwargs )
    try:
        return  subprocess.run( command, **params ) # type: ignore
    except subprocess.CalledProcessError as e:
        error_msg = get_process_text( e ).strip( ) or str( e )
        cmd_str = " ".join( e.cmd ) if isinstance( e.cmd, list ) else str( e.cmd )
        raise   Exception( f"failed at command: {cmd_str}\nerror: {error_msg}" ) from None


def get_process_text( result: subprocess.CompletedProcess | subprocess.CalledProcessError ) -> str:
    return  "\n".join( [ result.stderr or "", result.stdout or "" ] )


def get_git_config( configuration_name: str ) -> str:
    return  create_process( [ "git", "config", "--global", configuration_name ] ).stdout.strip( )


def get_cpu_count( ) -> int:
    #   termux: os.cpu_count( ) does not work on android
    try:
        return  len( os.sched_getaffinity( 0 ) ) # type: ignore
    except AttributeError:
        return  os.cpu_count( ) or 1


def set_attributes( obj, data: dict ):
    for key, value in data.items( ):
        setattr( obj, key, value )


def get_json_args( ):
    try:
        return  json.loads( value_or( sys.argv, 1, "{ }" ) )
    except json.JSONDecodeError:
        ensure( False, "invalid json parameters" )


