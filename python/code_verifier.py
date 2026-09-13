#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


from python.python_lib.python_config import python_project_config
from python.python_lib.python_project_model import python_project_model
from python.python_lib.python_verifier import python_verifier
from tools.tool import tool


@tool( )
def code_verifier( files: list[ str ] ) -> str:
    """verifies if a list of python files follows the project's formatting rules
returns a consolidated list of violations"""
    model = python_project_model( python_project_config )
    return  python_verifier( model, files, flg_auto_fix = True ).run( )


