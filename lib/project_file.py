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
#   File:   lib/project_file.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-01-25 15:40:46
#   


#


from abc import ABC, abstractmethod
from datetime import datetime
from itertools import takewhile
import re

from sak.common import datetime_from_string, extract_string, remove_string_prefix
from sak.fso import text_file
from lib.project_config import project_config
from lib.template import template


line_break = project_config[ "system" ][ "pattern" ][ "line_break" ]
triple_line_break = line_break * 3


class project_file( text_file, ABC ):
    @property
    @abstractmethod
    def shebang( self ) -> str:
        pass

    @property
    @abstractmethod
    def comment_string( self ) -> str:
        pass

    @property
    def expected_shebang( self ) -> str:
        return  "" if not self.shebang else self.shebang + line_break * 2

    @property
    def license_header_info( self ) -> dict:
        info = {
             "name": project_config[ "author" ][ "name" ]
            ,"email": project_config[ "author" ][ "email" ]
            ,"created_at": ( self.created_at or datetime.now( ) ).timestamp( )
        }
        
        if not self.license_header:
            return  info
        
        comment = re.escape( self.comment_string )
        
        if author := extract_string( rf"{comment}.*Author:\s*(.*?)\s*<([^>]+)>", self.license_header ):
            email = author[ 1 ].strip( )
            if "@" in email:
                info[ "name" ] = author[ 0 ].strip( )
                info[ "email" ] = email
        
        if date_list := extract_string( rf"{comment}.*Created on\s*(.*)", self.license_header ):
            if date := datetime_from_string( project_config[ "locale" ][ "datetime-format" ], date_list[ 0 ].strip( ) ):
                info[ "created_at" ] = date.timestamp( )
        
        return  info

    @property
    def license_header( self ) -> str:
        if not ( self.content and self.comment_string ):
            return  None
        
        text = self.content
        comment = self.comment_string
        expected_shebang = self.expected_shebang
        
        if expected_shebang:
            if not text.startswith( expected_shebang ):
                return  None
            text = remove_string_prefix( text, expected_shebang )
        
        if not text.startswith( comment ):
            return  None
        
        return  expected_shebang + "".join( takewhile(
            lambda line: line.startswith( comment )
            ,text.splitlines( keepends = True )
        ) )

    @property
    def metadata( self ) -> dict:
        info = self.license_header_info
        created_at = datetime.fromtimestamp( info[ "created_at" ] )
        return  {
             "file_created_at_year": created_at.strftime( "%Y" )
            ,"file_author_name": info[ "name" ]
            ,"file_author_email": info[ "email" ]
            ,"file_created_at": created_at.strftime( project_config[ "locale" ][ "datetime-format" ] )
            ,"file_path": self.path
            ,"comment_string": self.comment_string
        }

    @property
    def expected_license_header( self ) -> str:
        return  self.expected_shebang + template( "file-header" ).render( self.metadata )

    @property
    def body( self ) -> str:
        result = remove_string_prefix( self.content, self.license_header or "" )
        result = re.sub( rf"^(?:[ \t]*{re.escape( line_break )})+", "", result )
        return  result.strip( line_break )

    def ensure_license_header( self ) -> None:
        if self.license_header == self.expected_license_header:
            return
        self.write(
                self.expected_license_header.strip( line_break )
            +   triple_line_break
            +   self.body.strip( line_break )
            +   triple_line_break
        )


