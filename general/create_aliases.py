#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


import os
from pathlib import Path

from sak.common import ensure, to_json, from_json, create_process
from tools.tool import tool


@tool( )
def create_aliases( ) -> str:
    """creates convenience symlinks for invoking project-mcp-tools tools via CLI"""
    project_root = os.environ[ "PROJECT_MCP_TOOLS_ROOT" ]
    target_root  = os.environ[ "PROJECT_MCP_TARGET_ROOT" ]
    rel_dir      = os.path.relpath( project_root, target_root )
    rel_target   = os.path.relpath( target_root, project_root )

    scripts_dir = Path( "scripts" )
    scripts_dir.mkdir( exist_ok = True )

    router_path = scripts_dir / "project-mcp-tools"
    router_path.write_text( f"""#!/bin/bash
tool_name="$(basename "$0")"
uv --quiet --directory "{rel_dir}" run cli --target-project \"$( pwd )\" "$tool_name" "$@" | jq -r '(. as $in | try (.data | fromjson) catch $in.data)'
""" )
    router_path.chmod( 0o755 )

    result = create_process( [
         "uv"
        ,"--directory", rel_dir
        ,"run", "cli"
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


