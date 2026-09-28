#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


import logging
import os

from google import genai
from google.genai import types

from sak.common import ensure

logging.getLogger( "google_genai._api_client" ).setLevel( logging.ERROR )


def _detect_mime_type( data: bytes ) -> str:
    if data.startswith( b"\x89PNG\r\n\x1a\n" ):
        return  "image/png"
    elif data.startswith( b"\xff\xd8\xff" ):
        return  "image/jpeg"
    elif data.startswith( b"RIFF" ) and data[ 8:12 ] == b"WEBP":
        return  "image/webp"
    elif data.startswith( b"GIF87a" ) or data.startswith( b"GIF89a" ):
        return  "image/gif"
    else:
        return  "image/png"


def _resolve_image_path( image_path: str ) -> str:
    target_root = os.environ.get( "PROJECT_MCP_TARGET_ROOT", os.getcwd( ) )
    return  os.path.join( target_root, image_path ) if not os.path.isabs( image_path ) else image_path


def interpret_image( image_path: str, description: str ) -> str:
    absolute_path = _resolve_image_path( image_path )
    ensure( os.path.isfile( absolute_path ), f"image not found in the target project: {image_path}" )

    with open( absolute_path, "rb" ) as f:
        image_data = f.read( )

    prompt = f"""interpret the image at "{image_path}" relative to the target project root and answer the question below
base the answer only on what is visible in the image, without guessing
question: {description}"""

    client = genai.Client( api_key = os.environ.get( "GEMINI_API_KEY" ) )
    response = client.models.generate_content(
         model   = "gemini-flash-lite-latest"
        ,contents = [
             prompt
            ,types.Part.from_bytes( data = image_data, mime_type = _detect_mime_type( image_data ) )
        ]
    )
    ensure( response.text, "model returned no text response" )
    return  response.text


