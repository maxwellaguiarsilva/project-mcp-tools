#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


from sak.common import ensure, create_process
from tools.tool import tool

@tool( )
def quick_upload( message: str ) -> str:
    """performs a quick git upload: pull, add all, commit with message, and push
this tool is intended for simple, non-conflicting changes to increase agility
inspect git status and git diff directly to produce the commit message using the Conventional Commits standard (do not use git log; history must not influence the commit message decision, only status and diff)
mandatory: the message must be in en-us"""
    create_process( [ "git", "pull" ] )
    create_process( [ "git", "add", "." ] )
    create_process( [ "git", "commit", "-m", message ] )
    create_process( [ "git", "push" ] )
    
    return  f"upload successful\n\n{message}"


