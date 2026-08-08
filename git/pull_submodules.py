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
#   File:   git/pull_submodules.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-16 22:55:54
#   


#


from tools.tool import tool
from git.git_lib.submodules import pull_all

@tool( )
def pull_submodules( ) -> str:
    """updates every submodule of the target project to the latest commit of its remote branch
each submodule must have a clean git status, otherwise the tool fails without touching anything
the updated pointers are left in the working tree so the owner decides when to commit the bump"""
    return  pull_all( )


