#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


from tools.tool import tool
from cpp.cpp_lib.build_runner import build_runner


@tool( )
def compile( ) -> str:
    """compiles the project using
this command takes no arguments"""
    return  build_runner( ).run( )


