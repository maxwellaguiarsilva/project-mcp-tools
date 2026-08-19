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
#   File:   git/quick_upload.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-01-16 13:59:50
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


