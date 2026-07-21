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
#   File:   python/clear.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-16 14:00:00
#   


import shutil
from pathlib import Path

from tools.tool import tool


@tool( )
def clear( ) -> str:
    """removes all __pycache__ directories under the current directory"""
    removed = 0
    for p in Path.cwd( ).rglob( "__pycache__" ):
        if p.is_dir( ):
            shutil.rmtree( p )
            removed += 1
    return  f"total __pycache__ directories removed: {removed}"


