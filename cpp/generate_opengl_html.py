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
#   File:   cpp/generate_opengl_html.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-08-17 10:00:00
#   


#


from tools.tool import tool
from cpp.cpp_lib.opengl_html import opengl_html_generator


@tool( )
def generate_opengl_html( ) -> str:
    """generates a single file html tree view of the opengl 4.6 core profile
reads the snake_case glad alias header from the target project
fetches the official khronos refpages into /tmp/generate-opengl-html
writes opengl.htm to the target project root"""
    return  opengl_html_generator( ).run( )


