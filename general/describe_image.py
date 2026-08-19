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
#   File:   general/describe_image.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-08-08 11:33:16
#   


from tools.tool import tool
from general.general_lib.image_interpreter import interpret_image


@tool( )
def describe_image( image_path: str, description: str ) -> str:
    """uses the Gemini vision model to interpret an image from the target project for models without vision capability
    image_path is the relative path of the image inside the target project
    description explains what the requesting model wants to know about the image
    the answer is based only on what is visible in the image"""
    return  interpret_image( image_path, description )


