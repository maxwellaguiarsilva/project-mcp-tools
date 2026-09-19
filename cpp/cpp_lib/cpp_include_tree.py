#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#

import re
from dataclasses import dataclass

from sak.common import ensure
from sak.fso import text_file
from .cpp_project_config import cpp_project_config
from .cpp_project_model import cpp_project_model


@dataclass
class redundant_include:
    owner: str
    include: str
    reason: str


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
    def __init__( self, file_path: str | None = None, flg_auto_fix: bool = False ):
        self.project = cpp_project_model( cpp_project_config )
        self.file_paths = [ file_path ] if file_path else sorted(
            [ path for path, item in self.project.files.items( ) if getattr( item, "is_main", False ) ]
        )
        ensure( self.file_paths, "could not determine a target file for include_tree analysis" )
        for target in self.file_paths:
            ensure( target in self.project.files, f"file {target} not found in project" )
        self.per_root = {
            target: self._collect_for_target( target ) for target in self.file_paths
        }
        if flg_auto_fix:
            for entries in self.per_root.values( ):
                for entry in entries:
                    self.remove_include( entry )

    def __repr__( self ) -> str:
        return  "\n".join( [ self._format_target( target ) for target in self.file_paths ] )

    def remove_include( self, entry: redundant_include ) -> bool:
        #   only handle canonical angle-bracket includes
        #   quoted or non-canonical shapes are left alone
        target = text_file( entry.owner )
        content = target.read( )
        if not content:
            return  False
        pattern = re.compile( r"""#include\s*<([^>]+)>""" )
        lines = content.splitlines( keepends = True )
        matches = [
            index
            for index, line in enumerate( lines )
            if ( found := pattern.search( line ) ) and found.group( 1 ).strip( ) == entry.include.strip( )
        ]
        if not matches:
            return  False
        lines = [ line for index, line in enumerate( lines ) if index != matches[ -1 ] ]
        target.write( "".join( lines ) )
        return  True

    def _collect_for_target( self, target: str ) -> list:
        node = self.project.files.get( target )
        collector = [ ]
        self._build_nodes( node, { target }, collector )
        return  sorted( collector, key = lambda entry: ( entry.owner, entry.include, entry.reason ) )

    def _format_target( self, target: str ) -> str:
        entries = self.per_root.get( target, [ ] )
        if not entries:
            return  f"- <{target}>\n    no redundant includes found"
        header = f"- <{target}>\n    owner | include | reason"
        rows = [ f"    {entry.owner} | <{entry.include}> | {entry.reason}" for entry in entries ]
        return  "\n".join( [ header ] + rows )

    def _build_nodes( self, node, branch_visited, collector ):
        children = [ include_node( include ) for include in node.includes ]
        
        for child, include in zip( children, node.includes ):
            header = self.project.get_file( include, is_header = True )
            if header and header.path not in branch_visited:
                child.items = self._build_nodes( header, branch_visited | { header.path }, collector )
        
        #   redundancy check
        for i, child in enumerate( children ):
            for j, sibling in enumerate( children ):
                if i == j:
                    continue
                
                if i > j and child.path == sibling.path:
                    child.is_redundant = True
                    collector.append(
                        redundant_include(
                             owner = node.path
                            ,include = child.path
                            ,reason = f"duplicate of sibling <{sibling.path}>"
                        )
                    )
                    break
                
                if child.path in sibling.get_descendants( ):
                    child.is_redundant = True
                    collector.append(
                        redundant_include(
                             owner = node.path
                            ,include = child.path
                            ,reason = f"found in subtree of sibling <{sibling.path}>"
                        )
                    )
                    break
        return  children
