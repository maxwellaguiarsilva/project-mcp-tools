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
#   File:   general/create_image.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-07 00:00:00
#   


#

from tools.tool import tool
from general.general_lib.image_generator import generate_image


@tool( )
def create_image( description: str, file_name: str | None = None ) -> str:
    """generates an image using Gemini from the given text description. do NOT include a file extension in file_name"""
    return  generate_image( description, file_name )


