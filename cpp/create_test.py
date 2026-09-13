#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


import re

from lib.template import template
from sak.common import ensure
from tools.tool import tool
from cpp.cpp_lib.cpp_project_config import cpp_project_config
from cpp.cpp_lib.cpp_project_model import cpp, cpp_project_model


@tool( )
def create_test( 
     hierarchy: str
    ,flg_adhoc: bool = False
    ,include_list: list[ str ] = [ ]
) -> str:
    """creates a new c++ test file
if flg_adhoc is true, creates an adhoc test in tests/adhoc/nnnn_hierarchy.cpp
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
        ,cpp( file_path, model ).metadata | {
             "hierarchy": hierarchy
            ,"include_list": include_list
        }
    )

    return  file_path


