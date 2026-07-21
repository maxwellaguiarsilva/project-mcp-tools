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
from git.git_lib.submodules import pull_all

@tool( )
def quick_upload( message: str ) -> str:
    """performs a quick git upload: pull, update submodules, add all, commit with message, and push
submodules are updated to the latest remote commit before staging, so the pointer bump is included
if any submodule is not clean the whole upload fails before anything is committed
this tool is intended for simple, non-conflicting changes to increase agility
the LLM must inspect git status, git diff, and git log -n3 on its own to produce the commit message
mandatory: the message must be in en-us"""
    create_process( [ "git", "pull" ] )
    submodules_report = pull_all( )
    create_process( [ "git", "add", "." ] )
    create_process( [ "git", "commit", "-m", message ] )
    create_process( [ "git", "push" ] )
    
    return  f"upload successful\n\n{message}\n\n{submodules_report}"


