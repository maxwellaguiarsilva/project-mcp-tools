#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
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


