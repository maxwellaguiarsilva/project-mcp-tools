#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


class cpp_check:
    default_config = {
         "executable": "cppcheck"
        ,"strictness": "exhaustive"
        ,"suppressions": [
             "missingIncludeSystem"
            ,"checkersReport"
        ]
    }

    def __init__( self, config, paths ):
        self.config = config
        self.paths = paths

    @property
    def suppressions( self ):
        suppressions = list( self.default_config[ "suppressions" ] )
        config_suppress = ( self.config.get( "quality_control", { } )
                                 .get( "static_analysis", { } )
                                 .get( "suppress", [ ] ) )
        suppressions.extend( config_suppress )
        return  suppressions

    @property
    def file_filters( self ):
        return  ( self.config.get( "quality_control", { } )
                  .get( "static_analysis", { } )
                  .get( "file-filter", [ ] ) )

    @property
    def flg_force( self ):
        return  bool( self.config.get( "quality_control", { } )
                           .get( "static_analysis", { } )
                           .get( "force", False ) )

    @property
    def command( self ):
        paths_str = " ".join( f'"{p}"' for p in self.paths )
        executable = self.default_config[ "executable" ]
        return  f"{executable} {self.params} {paths_str}"

    @property
    def params( self ):
        config = self.config
        
        params = [
            "--quiet"
            ,"--enable=all"
            ,f"--cppcheck-build-dir={config[ "paths" ][ "build" ]}"
            ,"--inline-suppr"
            ,f"--std={config[ "compiler" ][ "standard" ]}"
            ,"--error-exitcode=1"
            ,f"-j {config[ "max-threads" ]}"
        ]
        
        params.append( f"--check-level={self.default_config[ "strictness" ]}" )
            
        for suppression in self.suppressions:
            params.append( f"--suppress={suppression}" )

        for file_filter in self.file_filters:
            params.append( f"--file-filter={file_filter}" )

        if self.flg_force:
            params.append( "--force" )

        params.append( f"-I{config[ "paths" ][ "include" ]}" )
        for d in config[ "dependencies" ][ "include-dirs" ]:
            params.append( f"-I{d}" )
            
        return  " ".join( params )


