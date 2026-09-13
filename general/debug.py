#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


import os
import sys
from pathlib import Path

from sak.common import to_json
from tools.tool import tool


@tool( )
def debug( ) -> str:
    """returns environment debugging information (cwd, paths, env vars)"""
    project_root_env = "PROJECT_MCP_TOOLS_ROOT"
    target_root_env = "PROJECT_MCP_TARGET_ROOT"
    return  to_json( {
         "cwd": str( Path.cwd( ) )
        ,"sys_path": sys.path
        ,"project_mcp_tools_root_env": os.environ.get( project_root_env, "(not set)" )
        ,"project_mcp_target_root_env": os.environ.get( target_root_env, "(not set)" )
        ,"project_mcp_tools_root_in_sys_path": any(
            project_root_env in p for p in sys.path
        )
        ,"python_executable": sys.executable
        ,"python_version": sys.version
        ,"environment_variables": {
            k: v
            for k, v in os.environ.items( )
            if k in (
                project_root_env
                ,target_root_env
                ,"PYTHONPATH"
                ,"PATH"
                ,"VIRTUAL_ENV"
                ,"UV_ROOT"
            )
        }
    } )


