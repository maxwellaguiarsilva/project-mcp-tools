#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
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
        return  self.project.config[ "language" ][ "comment-string" ]


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


