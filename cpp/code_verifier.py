#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#



from tools.tool import tool
from cpp.cpp_lib.cpp_project_config import cpp_project_config
from cpp.cpp_lib.cpp_project_model import cpp_project_model
from cpp.cpp_lib.cpp_verifier import cpp_verifier


@tool( )
def code_verifier( files: list[ str ] = [ ] ) -> str:
    """verifies if a list of files follows the project's formatting rules
returns a consolidated list of violations
to verify and process the entire project, prefer the `cpp_analyze` tool
the `cpp_code_verifier` tool is recommended for a small group of files or just a single file"""
    model = cpp_project_model( cpp_project_config )
    return  cpp_verifier( model, files, flg_auto_fix = True ).run( )


