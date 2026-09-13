#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


import os

from sak.fso.file import file


class text_file( file ):
    #   do not display self.content in __repr__ or json

    def __init__( self, file_path ):
        self.content    =   None
        super( ).__init__( file_path )
    
    def refresh( self ):
        super( ).refresh( )
        self.read( )
    
    def read( self ):
        if self.exists:
            with open( self.path, "r", encoding="utf-8" ) as input_file:
                self.content    =   input_file.read( )
        return  self.content
    
    def write( self, content ):
        dir_name = os.path.dirname( self.path )
        if dir_name:
            os.makedirs( dir_name, exist_ok = True )
        with open( self.path, "w", encoding="utf-8" ) as output_file:
            output_file.write( content )
        self.refresh( )
        return  f"created file: {self.path}\n"


