#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


from sak.common import ensure, create_process
from tools.tool import tool

@tool( )
def discard_changes( ) -> str:
    """discards all uncommitted changes and removes untracked files
this tool reverts the repository to the state of the last commit (head)"""
    create_process( [ "git", "reset", "--hard", "HEAD" ] )
    create_process( [ "git", "clean", "-fd" ] )
        
    return  f"all uncommitted changes have been discarded and untracked files removed"


