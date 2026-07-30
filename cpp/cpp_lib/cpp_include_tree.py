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
#   File:   cpp/cpp_lib/cpp_include_tree.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-07 00:00:00
#   


#

from sak.common import ensure
from .cpp_project_model import cpp_project_model
from .cpp_project_config import cpp_project_config


class include_node:
    def __init__( self, path: str, is_redundant: bool = False ):
        self.path = path
        self.items = [ ]
        self.is_redundant = is_redundant

    def __repr__( self ) -> str:
        text = f"<{self.path}>"
        if self.is_redundant:
            return  f"{text} (redundant)"
        
        if not self.items:
            return  text
            
        return  "\n".join( [ text ] + [ "    " + repr( item ).replace( "\n", "\n    " ) for item in self.items ] )

    def get_descendants( self ) -> set:
        descendants = set( )
        for item in self.items:
            descendants.add( item.path )
            descendants.update( item.get_descendants( ) )
        return  descendants


class include_tree:
    def __init__( self, file_path: str = None ):
        self.project = cpp_project_model( cpp_project_config )
        self.file_path = file_path
        
        if not self.file_path:
            for f in self.project.files.values( ):
                if hasattr( f, "is_main" ) and f.is_main and not getattr( f, "is_test", False ):
                    self.file_path = f.path
                    break
        
        ensure( self.file_path, "could not determine a target file for include_tree analysis" )
        self.node = self.project.files.get( self.file_path )
        ensure( self.node, f"file {self.file_path} not found in project" )

    def __repr__( self ) -> str:
        root_children = self._build_nodes( self.node, { self.file_path } )
        return  "\n".join( [ f"- <{self.file_path}>" ] + [ "    " + repr( child ).replace( "\n", "\n    " ) for child in root_children ] )

    def _build_nodes( self, node, branch_visited ):
        children = [ include_node( include ) for include in node.includes ]
        
        for child, include in zip( children, node.includes ):
            header = self.project.get_file( include, is_header = True )
            if header and header.path not in branch_visited:
                child.items = self._build_nodes( header, branch_visited | { header.path } )
        
        #   redundancy check
        for i, child in enumerate( children ):
            for j, sibling in enumerate( children ):
                if i == j:
                    continue
                
                if i > j and child.path == sibling.path:
                    child.is_redundant = True
                    break
                
                if child.path in sibling.get_descendants( ):
                    child.is_redundant = True
                    break
        return  children


