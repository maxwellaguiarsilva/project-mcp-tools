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
#   File:   git/discard_changes.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-01-16 22:34:01
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


