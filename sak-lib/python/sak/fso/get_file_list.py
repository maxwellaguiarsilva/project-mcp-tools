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
#   File:   lib/fso/get_file_list.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-01-24 16:29:35
#   


import os

from sak.common import create_process, ensure_list

def get_file_list( path: str, extensions = None, flg_recursive = True ):
    ext_list = [ ext if ext.startswith( "." ) else f".{ext}" for ext in ensure_list( extensions, str ) ] if extensions else None

    result = create_process( [ "git", "ls-files", "--cached", "--others", "--exclude-standard", path ], check = False )
    all_files = result.stdout.strip( ).split( "\n" ) if result.stdout.strip( ) else [ ]

    files = [
        os.path.normpath( file_path )
        for file_path in all_files
        if file_path
        and ( ext_list is None or any( file_path.endswith( ext ) for ext in ext_list ) )
        and ( flg_recursive or os.sep not in os.path.relpath( file_path, path ) )
    ]
 
    return  files


