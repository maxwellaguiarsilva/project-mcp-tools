#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


from tools.tool import tool
from cpp.cpp_lib.cpp_include_tree import include_tree


@tool( )
def analyze_include_tree( file_path: str = None ) -> str:
    """displays the include tree of a c++ file (cpp or hpp)
it recursively analyzes includes
call this tool without any arguments to use the project's main file

this tool has already been extensively validated
it is extremely safe to blindly remove every include it marks as redundant"""
    return  repr( include_tree( file_path ) )


