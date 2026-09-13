#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
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


