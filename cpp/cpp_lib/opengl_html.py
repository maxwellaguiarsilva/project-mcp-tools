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
#   File:   cpp/cpp_lib/opengl_html.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-08-17 10:00:00
#   


#


import glob
import html
import os
import re

from lib.template import template
from sak.common import create_process, ensure, to_json
from sak.fso import text_file


class gl_function:
    def __init__( self, snake_name: str, camel_name: str ):
        self.snake_name = snake_name
        self.camel_name = camel_name
        self.refpurpose = ""
        self.description = ""


class snake_header_reader:
    define_regex = re.compile( r"""^#define\s+(gl_[a-z0-9_]+)\s+glad_(\w+)\s*$""", re.MULTILINE )

    def __init__( self, header_path: str ):
        content = text_file( header_path ).content or ""
        ensure( content, f"snake_case header not found: {header_path}" )
        self.functions = [
            gl_function( snake_name, camel_name )
            for snake_name, camel_name in sorted( self.define_regex.findall( content ) )
        ]


class refpages_source:
    repo_url = "https://github.com/KhronosGroup/OpenGL-Refpages.git"
    work_dir = os.path.join( os.sep, "tmp", "generate-opengl-html" )

    def __init__( self ):
        self.repo_dir = os.path.join( self.work_dir, "OpenGL-Refpages" )
        self.gl4_dir = os.path.join( self.repo_dir, "gl4" )
        self.ensure_cloned( )

    def ensure_cloned( self ):
        probe = text_file( os.path.join( self.gl4_dir, "glActiveTexture.xml" ) )
        if probe.exists:
            create_process( [ "git", "-C", self.repo_dir, "pull", "--ff-only" ], check = False )
            return
        create_process( [ "git", "clone", "--depth", "1", self.repo_url, self.repo_dir ] )


class entity_map:
    define_regex = re.compile( r"""<!ENTITY\s+(\w+)\s+"([^"]*)"\s*>""" )
    name_regex = re.compile( r"&(\w+);" )

    def __init__( self, ent_files: list ):
        self.map = {
            name: html.unescape( value )
            for ent_file in ent_files
            for name, value in self.define_regex.findall( text_file( ent_file ).content or "" )
        }

    def resolve( self, text: str ) -> str:
        text = html.unescape( text )
        return  self.name_regex.sub( lambda match: self.map.get( match.group( 1 ), "" ), text )


class refpage_doc:
    tag_regex = re.compile( r"<[^>]+>" )
    section_regex = re.compile( r"""<refsect1 xml:id="description">(.*?)</refsect1>""", re.S )
    para_regex = re.compile( r"<para>(.*?)</para>", re.S )
    synopsis_regex = re.compile( r"<refsynopsisdiv>.*?</refsynopsisdiv>", re.S )
    function_regex = re.compile( r"<function>(\w+)</function>" )

    def __init__( self, file_path: str, content: str, entities: entity_map ):
        self.entities = entities
        self.name = os.path.splitext( os.path.basename( file_path ) )[ 0 ]
        self.refpurpose = self.extract_single( content, "refpurpose" )
        self.description = self.extract_description( content )
        synopsis = self.synopsis_regex.search( content )
        self.synopsis_functions = (
            sorted( set( self.function_regex.findall( synopsis.group( 0 ) ) ) )
            if synopsis
            else [ ]
        )

    def clean( self, text: str ) -> str:
        text = self.tag_regex.sub( " ", text )
        return  self.entities.resolve( " ".join( text.split( ) ) )

    def extract_single( self, content: str, tag: str ) -> str:
        match = re.search( rf"<{tag}>(.*?)</{tag}>", content, re.S )
        return  self.clean( match.group( 1 ) ) if match else ""

    def extract_description( self, content: str ) -> str:
        section = self.section_regex.search( content )
        if not section:
            return  ""
        paragraphs = [ self.clean( para ) for para in self.para_regex.findall( section.group( 1 ) ) ]
        return  "\n\n".join( [ para for para in paragraphs if para ] )


class refpage_index:
    def __init__( self, gl4_dir: str ):
        ent_names = [ "isonum.ent", "isoamsb.ent", "isoamsc.ent", "isotech.ent", "isogrk3.ent", "math.ent" ]
        self.entities = entity_map( [ os.path.join( gl4_dir, name ) for name in ent_names ] )
        self.pages = { }
        for xml_path in sorted( glob.glob( os.path.join( gl4_dir, "*.xml" ) ) ):
            content = text_file( xml_path ).content or ""
            if "<refsynopsisdiv>" not in content:
                continue
            page = refpage_doc( xml_path, content, self.entities )
            for function_name in page.synopsis_functions:
                self.pages.setdefault( function_name, page )

    def resolve( self, camel_name: str ) -> tuple | None:
        page = self.pages.get( camel_name )
        return  ( page.refpurpose, page.description ) if page else None


class gl_tree_node:
    def __init__( self, label: str ):
        self.label = label
        self.function = None
        self.children = { }

    def insert( self, segments: list, function: gl_function ):
        if not segments:
            self.function = function
            return
        head, tail = segments[ 0 ], segments[ 1: ]
        child = self.children.setdefault( head, gl_tree_node( head ) )
        child.insert( tail, function )


def leaf_item( function: gl_function ) -> dict:
    return  {
         "snake": function.snake_name
        ,"camel": function.camel_name
        ,"refpurpose": function.refpurpose
        ,"description": function.description
    }


def compress_node( node: gl_tree_node ) -> dict:
    items = ( [ leaf_item( node.function ) ] if node.function else [ ] ) + [
        compress_child( child ) for child in node.children.values( )
    ]
    return  {
         "label": node.label
        ,"count": sum( item.get( "count", 1 ) for item in items )
        ,"children": items
    }


def compress_child( child: gl_tree_node ) -> dict:
    merged = child
    while merged.function is None and len( merged.children ) == 1:
        only = next( iter( merged.children.values( ) ) )
        merged = gl_tree_node( f"{merged.label}_{only.label}" )
        merged.function = only.function
        merged.children = only.children
    if merged.function is not None and not merged.children:
        return  leaf_item( merged.function )
    return  compress_node( merged )


class opengl_html_generator:
    header_relative_path = os.path.join( "include", "glad", "snake_case.hpp" )
    output_relative_path = "opengl.htm"
    template_name = "opengl-html"

    def run( self ) -> str:
        functions = snake_header_reader( self.header_relative_path ).functions
        source = refpages_source( )
        index = refpage_index( source.gl4_dir )
        missing_docs = [ ]
        for function in functions:
            docs = index.resolve( function.camel_name )
            if docs:
                function.refpurpose, function.description = docs
            else:
                missing_docs.append( function.camel_name )

        root = gl_tree_node( "gl" )
        for function in functions:
            root.insert( function.snake_name.split( "_" )[ 1: ], function )

        data = {
             "total": len( functions )
            ,"missing_docs": missing_docs
            ,"root": compress_node( root )
        }
        json_data = to_json( data, flg_indent = False ).replace( "</", "<\\/" )
        html_text = template( self.template_name ).render( { "json_data": json_data } )
        created = text_file( self.output_relative_path ).write( html_text )
        return  (
            f"{created}"
            f"functions: {len( functions )}\n"
            f"documented: {len( functions ) - len( missing_docs )}\n"
            f"missing official refpage: {len( missing_docs )}\n"
        )


