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
#   File:   cpp/create_class.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-01-16 14:22:13
#   


#


import time

from sak.common import ensure
from tools.tool import tool
from lib.template import template
from cpp.cpp_lib.cpp_project_config import cpp_project_config
from cpp.cpp_lib.cpp_project_model import parse_hierarchy, cpp_project_model
from cpp.cpp_lib.cpp_verifier import cpp_verifier


@tool( )
def create_class( 
     class_hierarchy: str
    ,include_list: list[ str ] = [ ]
    ,using_list: list[ str ] = [ ]
    ,create_header_only: bool = False 
) -> str:
    """creates a new c++ class with corresponding .hpp and .cpp files
the class_hierarchy parameter defines the namespace and class name (e.g., "game/player" creates class "player" in namespace "game")
optional include_list and using_list parameters allow specifying additional headers to include and "using" declarations to add
good example: include_list=["string", "vector"], using_list=[ "::std::string", "::std::vector", "item_list   =   vector< string >"]
bad example: include_list="<string>", using_list="using std::string;" """
    hierarchy_list = parse_hierarchy( class_hierarchy )
    model = cpp_project_model( cpp_project_config )
    
    header_path = model.get_path_for_hierarchy( class_hierarchy, "header" )
    
    template( "cpp/class-hpp" ).create_file( 
         header_path
        ,{
             "comment_string": cpp_project_config[ "language" ][ "comment_string" ]
            ,"header_guard": f"header_guard_{ str( time.time_ns( ) )[ -9: ] }"
            ,"class_name": hierarchy_list[ -1 ]
            ,"include_list": include_list
            ,"namespace_list": hierarchy_list[ :-1 ]
            ,"using_list": using_list
        }
    )

    if ( create_header_only ):
        return  cpp_verifier( model, [ header_path ], flg_auto_fix = True ).run( )
    
    source_path = model.get_path_for_hierarchy( class_hierarchy, "source" )
    rel_header_path = "/".join( hierarchy_list ) + "." + cpp_project_config[ "language" ][ "header_extension" ]

    template( "cpp/class-cpp" ).create_file( 
         source_path
        ,{
             "comment_string": cpp_project_config[ "language" ][ "comment_string" ]
            ,"include_list": [ rel_header_path ]
        }
    )

    return  cpp_verifier( model, [ header_path, source_path ], flg_auto_fix = True ).run( )


