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
#   File:   python/code_verifier.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-01-18 16:21:38
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


