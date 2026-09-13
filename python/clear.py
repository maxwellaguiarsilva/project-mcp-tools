#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
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


