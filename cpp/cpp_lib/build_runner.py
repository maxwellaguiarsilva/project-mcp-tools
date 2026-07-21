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
#   File:   cpp/cpp_lib/build_runner.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-16 12:00:00
#   


from datetime import datetime
import os
from os.path import dirname
import concurrent.futures

from sak.common import get_cpu_count
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
        max_workers = self.core.config.get( "max_threads", get_cpu_count( ) )
        self.core.print( f"\ncompiling {len(all_cpps)} files using {max_workers} threads..." )
        
        with concurrent.futures.ThreadPoolExecutor( max_workers = max_workers ) as executor:
            futures = [ executor.submit( self.core.build, c ) for c in all_cpps.values( ) ]
            try:
                for future in concurrent.futures.as_completed( futures ):
                    future.result( )
            except Exception as e:
                self.core.stop( )
                raise   e

        #   4. parallel linking
        self.core.print( f"\nlinking {len(self.core.binary_list)} binaries using {max_workers} threads..." )
        with concurrent.futures.ThreadPoolExecutor( max_workers = max_workers ) as executor:
            futures = [ executor.submit( b.link ) for b in self.core.binary_list ]
            try:
                for future in concurrent.futures.as_completed( futures ):
                    future.result( )
            except Exception as e:
                self.core.stop( )
                raise   e

        end_time = datetime.now( )
        elapsed_time = end_time - start_time
        self.core.print( f"""\nbuild ended at: {end_time.strftime( "%Y-%m-%d %H:%M:%S" )}""" )
        self.core.print( f"elapsed time: {elapsed_time}" )
        
        return  "compilation completed successfully"


