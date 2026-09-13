#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#

from tools.tool import tool
from general.general_lib.image_generator import generate_image


@tool( )
def create_image( description: str, file_name: str | None = None ) -> str:
    """generates an image using Gemini from the given text description. do NOT include a file extension in file_name"""
    return  generate_image( description, file_name )


