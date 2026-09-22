#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


from tools.tool import tool
from cpp.cpp_lib.opengl_html import opengl_html_generator


@tool( )
def generate_opengl_html( ) -> str:
    """generates a single file html tree view of the opengl 4.6 core profile
reads the snake_case glad alias header from the target project
fetches the official khronos refpages into /tmp/generate-opengl-html
writes opengl.html to the output directory defined in cpp_project_config"""
    return  opengl_html_generator( ).run( )


