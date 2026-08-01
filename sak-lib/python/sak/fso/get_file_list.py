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
#   File:   sak-lib/python/sak/fso/get_file_list.py
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

    files = [ ]
    for file_path in all_files:
        if not file_path:
            continue

        #   tracked symlink pointing to a directory: expand through it,
        #   reusing the target repo's own git tracking/ignore rules
        if os.path.islink( file_path ) and os.path.isdir( os.path.realpath( file_path ) ):
            files.extend( _expand_symlink_dir( file_path, ext_list, flg_recursive ) )
        elif _matches( file_path, ext_list, path, flg_recursive ):
            files.append( os.path.normpath( file_path ) )

    return  files


def _expand_symlink_dir( symlink_path, ext_list, flg_recursive ):
    real_dir = os.path.realpath( symlink_path )

    probe = create_process( [ "git", "-C", real_dir, "rev-parse", "--show-toplevel" ], check = False )
    if probe.returncode != 0:
        #   target is not inside a git worktree: nothing tracked to list
        return  [ ]

    result = create_process( [ "git", "-C", real_dir, "ls-files", "--cached", "--others", "--exclude-standard", "." ], check = False )
    raw_files = result.stdout.strip( ).split( "\n" ) if result.stdout.strip( ) else [ ]

    internal_files = [ ]
    for raw in raw_files:
        raw = os.path.normpath( raw )
        if not raw:
            continue

        virtual_path = os.path.join( symlink_path, raw )
        if _matches( virtual_path, ext_list, symlink_path, flg_recursive ):
            internal_files.append( virtual_path )

    return  internal_files


def _matches( file_path, ext_list, base_path, flg_recursive ):
    if ext_list is not None and not any( file_path.endswith( ext ) for ext in ext_list ):
        return  False
    if not flg_recursive and os.sep in os.path.relpath( file_path, base_path ):
        return  False
    return  True
