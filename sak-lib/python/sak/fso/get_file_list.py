#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
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


