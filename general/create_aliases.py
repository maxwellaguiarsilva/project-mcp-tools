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
#   File:   general/create_aliases.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-23 23:09:31
#   


from pathlib import Path

from sak.common import ensure, to_json, from_json, create_process
from tools.tool import tool


@tool( )
def create_aliases( ) -> str:
    """creates convenience symlinks for invoking project-mcp-tools tools via CLI"""
    scripts_dir = Path( "scripts" )
    scripts_dir.mkdir( exist_ok = True )

    router_path = scripts_dir / "project-mcp-tools"
    router_path.write_text( """#!/bin/bash
tool_name="$(basename "$0")"
uv --directory "project-mcp-tools" run cli --target-project "../" "$tool_name" "$@" | jq -r '.data | fromjson'
""" )
    router_path.chmod( 0o755 )

    result = create_process( [
         "uv"
        ,"--directory", "project-mcp-tools"
        ,"run", "cli"
        ,"--target-project", "."
        ,"list_tools"
    ] )
    outer = from_json( result.stdout )
    ensure( outer.get( "status" ) == "success", "failed to list tools" )
    inner = from_json( outer[ "data" ] )
    tool_names = [ t[ "name" ] for t in inner ]

    for entry in scripts_dir.iterdir( ):
        if entry.is_symlink( ):
            entry.unlink( )

    created = [ ]
    for name in tool_names:
        kebab_name = name.replace( "_", "-" )
        link_path = scripts_dir / kebab_name
        link_path.symlink_to( "project-mcp-tools" )
        created.append( kebab_name )

    return  to_json( {
         "count": len( created )
        ,"names": created
    } )
