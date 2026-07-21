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
#   File:   python/python_lib/python_project_model.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-02-01 23:45:00
#   


#


import os

from lib.project_file import project_file
from sak.common import to_dict, to_json
from sak.fso import get_file_list


class python_project_file( project_file ):
    def __init__( self, file_path, project ):
        self.project = project
        super( ).__init__( file_path )

    @property
    def shebang( self ):
        return  "#!/usr/bin/python3"

    @property
    def comment_string( self ):
        return  self.project.config[ "language" ][ "comment_string" ]


class python_project_model:
    def __init__( self, config ):
        self.config = config
        self.tools_dir = self.config[ "paths" ][ "tools" ]
        self.extension = self.config[ "language" ][ "extension" ]
        self.files = self.scan_dir( self.tools_dir )

    def scan_dir( self, dir_path ):
        return  {
            os.path.normpath( file_path ): python_project_file( file_path, self )
            for file_path in get_file_list( dir_path, extensions = [ self.extension ] )
        }

    def get_files( self, files = [ ] ):
        if not files:
            return  list( self.files.values( ) )
        
        return  [
            self.files[ candidate ]
            for p in files
            if ( candidate := next(
                ( c for c in (
                    os.path.normpath( p ),
                    os.path.normpath( os.path.join( self.tools_dir, p ) )
                ) if c in self.files ),
                None
            ) ) is not None
        ]

    @property
    def to_dict( self ):
        return  to_dict( self, [ "files" ] )

    def __repr__( self ):
        return  to_json( self.to_dict )


