#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


from sak.common import print_line

from tools.tool import tool
from cpp.cpp_lib.project_core import project_core
from cpp.cpp_lib.cpp_verifier import cpp_verifier


# @tool( )
# disabled at the user's request on 2026-09-12 16:35:53
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


