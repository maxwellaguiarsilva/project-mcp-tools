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
#   File:   cpp/create_test.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-01-16 14:22:13
#   


#


import re

from lib.template import template
from sak.common import ensure
from tools.tool import tool
from cpp.cpp_lib.cpp_project_config import cpp_project_config
from cpp.cpp_lib.cpp_project_model import cpp_project_model
from cpp.cpp_lib.cpp_verifier import cpp_verifier


@tool( )
def create_test( 
     hierarchy: str
    ,flg_adhoc: bool = False
    ,include_list: list[ str ] = [ ]
) -> str:
    """creates a new c++ test file
if flg_adhoc is true, creates an adhoc test in tests/adhoc/nnnn_hierarchy/
in adhoc mode, "hierarchy" must be a simple name (no slashes or paths)
if flg_adhoc is false, creates a structured test in tests/path/test_path_hierarchy.cpp"""
    ensure( not re.match( r"^\d+_", hierarchy ), "numeric prefix is managed by the system, do not provide it manually" )
    
    model = cpp_project_model( cpp_project_config )
    
    file_path = model.get_path_for_hierarchy( 
         hierarchy
        ,"test"
        ,flg_adhoc = flg_adhoc 
    )

    template( "cpp/test-cpp" ).create_file( 
         file_path
        ,{
             "comment_string": cpp_project_config[ "language" ][ "comment_string" ]
            ,"hierarchy": hierarchy
            ,"include_list": include_list
        }
    )

    return  cpp_verifier( model, [ file_path ], flg_auto_fix = True ).run( )


