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
#   File:   general/debug.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-10 01:00:00
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


