#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
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


