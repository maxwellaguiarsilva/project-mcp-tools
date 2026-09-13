#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


from datetime import datetime
import os
from os.path import dirname
import concurrent.futures

from sak.common import ensure, get_cpu_count, line_thick
from cpp.cpp_lib.project_core import project_core


class build_runner:
    def __init__( self ):
        self.core = project_core( { } )

    def run( self ) -> str:
        start_time = datetime.now( )

        self.core.print( f"""build started at: {start_time.strftime( "%Y-%m-%d %H:%M:%S" )}""" )

        #   1. collect all unique cpp files to build
        all_cpps = { c.path: c for b in self.core.binary_list for c in b.dependencies_list }

        #   2. ensure build directories exist
        for c in all_cpps.values( ):
            os.makedirs( dirname( c.object.path ), exist_ok = True )

        #   3. parallel compilation
        max_workers = self.core.config.get( "max-threads", get_cpu_count( ) )
        self.core.print( f"\ncompiling {len(all_cpps)} files using {max_workers} threads..." )
        
        with concurrent.futures.ThreadPoolExecutor( max_workers = max_workers ) as executor:
            futures = [ executor.submit( self.core.build, c ) for c in all_cpps.values( ) ]
            for future in concurrent.futures.as_completed( futures ):
                future.result( )

        if self.core.build_errors:
            separator = f"\n\n{line_thick}\n\n"
            ensure( False, f"""{separator.join( self.core.build_errors )}""" )

        #   4. parallel linking
        self.core.print( f"\nlinking {len(self.core.binary_list)} binaries using {max_workers} threads..." )
        with concurrent.futures.ThreadPoolExecutor( max_workers = max_workers ) as executor:
            futures = [ executor.submit( b.link ) for b in self.core.binary_list ]
            for future in concurrent.futures.as_completed( futures ):
                future.result( )

        if self.core.build_errors:
            separator = f"\n\n{line_thick}\n\n"
            ensure( False, f"""{separator.join( self.core.build_errors )}""" )

        end_time = datetime.now( )
        elapsed_time = end_time - start_time
        self.core.print( f"""\nbuild ended at: {end_time.strftime( "%Y-%m-%d %H:%M:%S" )}""" )
        self.core.print( f"elapsed time: {elapsed_time}" )
        
        self.core.flush_logs( )
        return  "compilation completed successfully"


