#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
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


