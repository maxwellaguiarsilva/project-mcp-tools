#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


class cpp_compiler:
    optimization_levels = {
        "none": "-O0"
        ,"balanced": "-O2"
        ,"aggressive": "-O3"
        ,"debug": "-Og"
    }

    warning_levels = {
        "minimal": [ "-Wall" ]
        ,"high": [ "-Wall", "-Wextra" ]
        ,"pedantic": [ "-Wall", "-Wextra", "-Wpedantic" ]
    }

    def __init__( self, config ):
        self.config = config

    def get_compile_command( self, cpp_file ):
        config = cpp_file.config
        params = self._get_compile_params( config )
        executable = config[ "compiler" ][ "executable" ]
        return  f"{executable} {params} -c {cpp_file.path} -o {cpp_file.object.path}"

    def get_link_command( self, cpp_file, object_files ):
        config = cpp_file.config
        params = self._get_link_params( config )
        executable = config[ "compiler" ][ "executable" ]
        object_files_str = " ".join( object_files )
        return  f"{executable} {object_files_str} {params} -o {cpp_file.binary.path}"

    def _get_compile_params( self, config ):
        params = [ ]
        
        params.append( f"-std={config[ "compiler" ][ "standard" ]}" )
        if config[ "compiler" ][ "use-64-bits" ]:
            params.append( "-m64" )
            
        opt_level = config[ "build_behavior" ][ "optimization" ]
        params.append( self.optimization_levels.get( opt_level, opt_level ) )
        
        if config[ "build_behavior" ][ "debug-symbols" ]:
            params.append( "-g" )
        if config[ "build_behavior" ][ "generate-dependencies" ]:
            params.append( "-MMD -MP" )
        if config[ "build_behavior" ][ "experimental-library" ]:
            params.append( "-fexperimental-library" )
            
        warn_level = config[ "quality_control" ][ "warning-level" ]
        params.extend( self.warning_levels.get( warn_level, [ warn_level ] ) )
        
        if config[ "quality_control" ][ "treat-warnings-as-errors" ]:
            params.append( "-Werror" )
        if config[ "quality_control" ][ "stop-on-first-error" ]:
            params.append( "-Wfatal-errors" )
            
        params.append( f"-I{config[ "paths" ][ "include" ]}" )
        for d in config[ "dependencies" ][ "include-dirs" ]:
            params.append( f"-I{d}" )
            
        params.extend( config[ "compiler" ].get( "extra-compile-flags", [ ] ) )

        return  " ".join( params )

    def _get_link_params( self, config ):
        params = [ ]
        
        if config[ "compiler" ][ "use-64-bits" ]:
            params.append( "-m64" )
            
        opt_level = config[ "build_behavior" ][ "optimization" ]
        params.append( self.optimization_levels.get( opt_level, opt_level ) )

        if config[ "build_behavior" ][ "debug-symbols" ]:
            params.append( "-g" )

        for d in config[ "dependencies" ].get( "library-dirs", [ ] ):
            params.append( f"-L{d}" )

        for opt in config[ "compiler" ].get( "linker-direct-options", [ ] ):
            params.append( f"-Wl,{opt}" )

        params.extend( config[ "compiler" ].get( "extra-link-flags", [ ] ) )

        for lib in config[ "dependencies" ][ "libraries" ]:
            params.append( f"-l{lib}" )

        return  " ".join( params )


