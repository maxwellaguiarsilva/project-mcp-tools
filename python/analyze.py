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
#   File:   python/analyze.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-01-24 15:51:01
#   


#


from sak.common import print_line
from python.python_lib.python_config import python_project_config
from python.python_lib.python_project_model import python_project_model
from python.python_lib.python_verifier import python_verifier
from tools.tool import tool


@tool( )
def analyze( ) -> str:
    """applies python_code_verifier in all *.py files found inside tools"""
    model = python_project_model( python_project_config )
    
    print_line( strong = False )
    print( f"checking python code formatting for {len( model.files )} files..." )
    print( python_verifier( model, flg_auto_fix = True ).run( ) )
    print_line( strong = False )

    return  "analysis completed successfully"


