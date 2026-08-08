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
#   File:   tools/path_manager.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-15 14:00:00
#   


import os
import sys
from pathlib import Path


_project_root_env = "PROJECT_MCP_TOOLS_ROOT"
_target_root_env = "PROJECT_MCP_TARGET_ROOT"


class path_manager:
    def __init__( self, project_root: Path | None = None, target_root: Path | None = None ):
        env_project = os.environ.get( _project_root_env )
        env_target = os.environ.get( _target_root_env )
        self._project_root = (
            project_root
            or ( Path( env_project ) if env_project else None )
            or Path( __file__ ).resolve( ).parent.parent
        )
        self._target_root = (
            target_root
            or ( Path( env_target ) if env_target else None )
            or Path.cwd( )
        )

    @property
    def project_root( self ) -> str:
        return  str( self._project_root )

    @property
    def target_root( self ) -> str:
        return  str( self._target_root )

    @target_root.setter
    def target_root( self, path: str | Path ):
        self._target_root = Path( path ).resolve( )

    def ensure_path( self ):
        root = str( self._project_root )
        if root not in sys.path:
            sys.path.insert( 0, root )

    def to_env( self ) -> dict[ str, str ]:
        return  {
             _project_root_env: str( self._project_root )
            ,_target_root_env: str( self._target_root )
        }


