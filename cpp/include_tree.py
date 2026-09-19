#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


from tools.tool import tool
from cpp.cpp_lib.cpp_include_tree import include_tree as include_tree_impl


@tool( )
def include_tree( file_path: str | None = None, flg_auto_fix: bool = False ) -> str:
    """reports redundant includes in c++ files (cpp or hpp)
it recursively analyzes includes
call this tool without any arguments to analyze every main file including tests in sorted path order
call with file_path to analyze exactly that file
it prints a table with owner file, redundant include and justification per entry
use flg_auto_fix to remove canonical angle-bracket includes and print the same table

this tool has already been extensively validated
it is extremely safe to blindly remove every include it marks as redundant"""
    return  repr( include_tree_impl( file_path, flg_auto_fix ) )

