#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


import re


from lib.project_config import project_config
from sak.common import ensure
from sak.fso import text_file


r_import        =   r"\{\{import\s+([a-zA-Z0-9_/-]+)\}\}"


class template:
    def __init__( self, name, path = None ):
        ensure( name != "", "template name cannot be empty" )
        self.path           =   path if path is not None else project_config[ "paths" ][ "templates" ]
        self.text           =   self.load( name )
    
    def load( self, name ):
        text    =   text_file( f"{self.path}/{name}.txt" ).content
        
        def resolve_imports( content ):
            return  re.sub( r_import, lambda match: resolve_imports( self.load_raw( match.group( 1 ) ) ), content )
        
        return  resolve_imports( text )

    def load_raw( self, name ):
        return  text_file( f"{self.path}/{name}.txt" ).content

    def _render_dict( self, data: dict, text: str ) -> str:
        for key, value in data.items( ):
            if isinstance( value, str ):
                text    =   text.replace( f"{{{{{key}}}}}", str( value ) )
            elif isinstance( value, list ):
                r_key   =   (
                        r"\{\{list_open\s+"
                    +   re.escape( key )
                    +   r"\}\}(.*?)\{\{list_close\s+"
                    +   re.escape( key )
                    +   r"\}\}"
                )
                r_item  =   (
                        r"\{\{list_item\s+"
                    +   re.escape( key )
                    +   r"\}\}"
                )
                text    =   re.sub(
                     r_key
                    ,lambda match: "".join( [ re.sub( r_item, str( item ), match.group( 1 ) ) for item in value ] )
                    ,text
                    ,flags = re.DOTALL
                )
            elif isinstance( value, dict ):
                text    =   self._render_dict( value, text )
        return  text

    def render( self, data: dict ) -> str:
        return  self._render_dict( data, self.text )

    def create_file( self, file_path, data ):
        return  text_file( file_path ).write( self.render( data ) )


