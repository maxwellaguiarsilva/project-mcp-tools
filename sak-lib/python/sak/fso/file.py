#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


from datetime import datetime
from os.path import basename, dirname, exists, getctime, getmtime, splitext

from sak.common import to_dict, to_json


class file:
    def __init__( self, file_path ):
        self.path   =   file_path
    
    @property
    def path( self ):
        return  self.file_path

    @path.setter
    def path( self, path ):
        name, extension =   splitext( basename( path ) )
        self.file_path  =   path
        self.base       =   dirname( path )
        self.folder     =   basename( self.base )
        self.name       =   name
        self.extension  =   extension[ 1: ]
        self.refresh( )
    
    def refresh( self ):
        self.exists         =   exists( self.path )
        self.modified_at    =   datetime.fromtimestamp( getmtime( self.path ) ) if self.exists else None
        self.created_at     =   datetime.fromtimestamp( getctime( self.path ) ) if self.exists else None

    @property
    def to_dict( self ):
        return  to_dict( self, {
             "path"
            ,"base"
            ,"folder"
            ,"name"
            ,"extension"
            ,"exists"
            ,"modified_at"
            ,"created_at"
        } )

    def __repr__( self ):
        return  to_json( self.to_dict )


