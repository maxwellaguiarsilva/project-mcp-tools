#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


import os
from pathlib import Path

from sak.common import ensure, create_process
from tools.path_manager import path_manager


def perform_upload( message: str, project_name: str | None = None ) -> str:
    if project_name is not None:
        project_path = Path( path_manager( ).target_root ).parent / project_name
        ensure( project_path.is_dir( ), f"project not found: { project_path }" )
        os.chdir( project_path )
    create_process( [ "git", "pull" ] )
    create_process( [ "git", "add", "." ] )
    create_process( [ "git", "commit", "-m", message ] )
    create_process( [ "git", "push" ] )
    
    return  f"upload successful\n\n{message}"


