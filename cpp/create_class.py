#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


import time

from sak.common import ensure
from tools.tool import tool
from lib.template import template
from cpp.cpp_lib.cpp_project_config import cpp_project_config
from cpp.cpp_lib.cpp_project_model import parse_hierarchy, cpp_project_model, hpp, cpp


@tool( )
def create_class( 
     class_hierarchy: str
    ,include_list: list[ str ] = [ ]
    ,using_list: list[ str ] = [ ]
    ,create_header_only: bool = False 
) -> str:
    """creates a new c++ class with corresponding .hpp and .cpp files
the class_hierarchy parameter defines the namespace and class name (e.g., "sak/foo/bar" creates class "::sak::for:bar" on file " on file "./include/sak/foo/bar.hpp" )
optional include_list and using_list parameters allow specifying additional headers to include and "using" declarations to add
good example: include_list=["string", "vector"], using_list=[ "::std::string", "::std::vector", "item_list   =   vector< string >"]
bad example: include_list="<string>", using_list="using std::string;" """
    hierarchy_list = parse_hierarchy( class_hierarchy )
    model = cpp_project_model( cpp_project_config )
    
    header_path = model.get_path_for_hierarchy( class_hierarchy, "header" )
    
    template( "cpp/class-hpp" ).create_file( 
         header_path
        ,hpp( header_path, model ).metadata | {
             "header_guard": f"header_guard_{ str( time.time_ns( ) )[ -9: ] }"
            ,"class_name": hierarchy_list[ -1 ]
            ,"include_list": include_list
            ,"namespace_list": hierarchy_list[ :-1 ]
            ,"using_list": using_list
        }
    )

    if ( create_header_only ):
        return  header_path
    
    source_path = model.get_path_for_hierarchy( class_hierarchy, "source" )
    rel_header_path = "/".join( hierarchy_list ) + "." + cpp_project_config[ "language" ][ "header-extension" ]

    template( "cpp/class-cpp" ).create_file( 
         source_path
        ,cpp( source_path, model ).metadata | {
             "include_list": [ rel_header_path ]
            ,"namespace_list": hierarchy_list[ :-1 ]
            ,"using_list": using_list
        }
    )

    return  header_path + "\n" + source_path


