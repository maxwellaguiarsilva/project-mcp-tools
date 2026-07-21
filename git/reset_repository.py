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
#   File:   git/reset_repository.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-20 01:45:24
#   


#

import shutil

from sak.common import ensure, create_process
from tools.tool import tool

@tool( )
def reset_repository( ) -> str:
    """resets the entire git repository: saves remote url, deletes .git, reinitializes,
stages all files in a single initial commit, re-adds the remote, sets upstream, and force pushes"""
    remote = create_process( [ "git", "remote", "get-url", "origin" ] ).stdout.strip( )
    ensure( remote, "no remote 'origin' found" )

    shutil.rmtree( ".git" )
    create_process( [ "git", "init" ] )
    create_process( [ "git", "add", "." ] )
    create_process( [ "git", "commit", "-m", "initial commit" ] )
    create_process( [ "git", "remote", "add", "origin", remote ] )
    create_process( [ "git", "branch", "-M", "main" ] )
    create_process( [ "git", "push", "-u", "origin", "main", "--force" ] )

    return  f"repository has been reset and force-pushed to {remote}"
