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
#   File:   git/git_lib/submodules.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-16 22:55:54
#   


#


from sak.common import ensure, create_process, remove_string_prefix


def tracked_branch( path: str ) -> str:
    result = create_process( [ "git", "-C", path, "symbolic-ref", "--short", "refs/remotes/origin/HEAD" ], check = False )
    return  remove_string_prefix( result.stdout.strip( ), "origin/" ) if result.returncode == 0 else "main"


def pull_submodule( path: str ) -> str:
    ensure(
         not create_process( [ "git", "-C", path, "status", "--porcelain" ] ).stdout.strip( )
        ,f"""submodule "{path}" has uncommitted changes and must be clean before pulling"""
    )
    branch = tracked_branch( path )
    create_process( [ "git", "-C", path, "switch", branch ] )
    create_process( [ "git", "-C", path, "pull", "--ff-only" ] )
    return  f"""submodule "{path}" now points to the latest "{branch}" commit from the remote"""


def pull_all( ) -> str:
    status_lines = create_process( [ "git", "submodule", "status" ] ).stdout.splitlines( )
    paths = [ line.split( )[ 1 ] for line in status_lines if line.strip( ) ]
    return  "\n".join( pull_submodule( path ) for path in paths ) if paths else "no submodules found"


