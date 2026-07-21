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
#   File:   cpp/analyze.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-01-16 14:22:13
#   


#


from sak.common import print_line

from tools.tool import tool
from cpp.cpp_lib.project_core import project_core
from cpp.cpp_lib.cpp_verifier import cpp_verifier


@tool( )
def analyze( ) -> str:
    """runs static analysis and automatically fixes formatting rules
beyond checking, it also applies fixes for the rules verified by "cpp_code_verifier" on all .cpp and .hpp files
this command takes no arguments"""
    core = project_core( { } )
    
    print_line( )
    core.print( f"checking code formatting for {len( core.map.files )} files..." )
    core.print( cpp_verifier( core.map, flg_auto_fix = True ).run( ) )
    core.print( "done formatting" )
    print_line( )

    core.run_static_analysis( )
    
    return  "analysis completed successfully"


