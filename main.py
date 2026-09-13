#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


import argparse
import sys

from tools.path_manager import path_manager
from tools.tool_manager import tool_manager
from sak.common import from_json, value_or

_parser = argparse.ArgumentParser( add_help = False )
_parser.add_argument( "--target-project", default = None, dest = "target_project" )

_path_manager = path_manager( )


def _parse_args( ):
    args, remaining = _parser.parse_known_args( sys.argv[ 1: ] )
    if args.target_project:
        _path_manager.target_root = args.target_project
    return  remaining

_path_manager.ensure_path( )

def _setup_manager( ) -> tool_manager:
    m = tool_manager( "project-mcp-tools", _path_manager )
    m.add_tool_folder( [ "git", "python", "cpp", "session" ] )
    m.add_tool_folder( "general", flg_use_prefix = False )
    m.add_tool_folder( "music", flg_use_prefix = False )
    return  m


def main_mcp( ):
    _parse_args( )
    _setup_manager( ).run_mcp( )


def main_api( ):
    _parse_args( )
    _setup_manager( ).run_api( )


def main_cli( ):
    _setup_manager( ).run_cli( _parse_args( ) )


if __name__ == "__main__":
    remaining = _parse_args( )
    if remaining:
        manager = _setup_manager( )
        tool_name = value_or( remaining, 0, "list-tools" )
        try:
            result = manager.execute_tool( tool_name, from_json( sys.stdin.read( ) ) )
            print( result )
        except Exception as e:
            print( str( e ), file = sys.stderr )
            sys.exit( 1 )
    else:
        print( """please use "uv run" to consume this project:""" )
        print( "  uv run mcp-server" )
        print( "  uv run api" )
        print( "  uv run cli" )
        sys.exit( 1 )


