#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


import base64
import os
import re

from google import genai

from sak.common import ensure


def _detect_image_format( data: bytes ) -> str:
    if data.startswith( b"\x89PNG\r\n\x1a\n" ):
        return  "png"
    elif data.startswith( b"\xff\xd8\xff" ):
        return  "jpg"
    elif data.startswith( b"RIFF" ) and data[ 8:12 ] == b"WEBP":
        return  "webp"
    elif data.startswith( b"GIF87a" ) or data.startswith( b"GIF89a" ):
        return  "gif"
    else:
        return  "png"


def _mime_type( format_name: str ) -> str:
    return  {
         "png": "image/png"
        ,"jpg": "image/jpeg"
        ,"webp": "image/webp"
        ,"gif": "image/gif"
    }[ format_name ]


def _resolve_image_path( image_path: str ) -> str:
    target_root = os.environ.get( "PROJECT_MCP_TARGET_ROOT", os.getcwd( ) )
    return  os.path.join( target_root, image_path ) if not os.path.isabs( image_path ) else image_path


def _build_input( description: str, images: list[ str ] | None ) -> str | list[ dict ]:
    if not images:
        return  description
    contents: list[ dict ] = [ { "type": "text", "text": description } ]
    for image_path in images:
        absolute_path = _resolve_image_path( image_path )
        ensure( os.path.isfile( absolute_path ), f"image not found in the target project: {image_path}" )
        with open( absolute_path, "rb" ) as file:
            image_data = file.read( )
        contents.append( {
             "type": "image"
            ,"data": base64.b64encode( image_data ).decode( "ascii" )
            ,"mime_type": _mime_type( _detect_image_format( image_data ) )
        } )
    return  contents


def _to_kebab_case( name: str ) -> str:
    name = name.lower( )
    name = re.sub( r"[^a-z0-9]+", "-", name )
    name = name.strip( "-" )
    return  name


def _next_image_name( directory: str ) -> str:
    max_counter = -1
    if os.path.isdir( directory ):
        for entry in os.listdir( directory ):
            base = entry.rsplit( ".", 1 )[ 0 ] if "." in entry else entry
            match = re.match( r"image-(\d{6})", base )
            if match:
                counter = int( match.group( 1 ) )
                if counter > max_counter:
                    max_counter = counter
    return  f"image-{max_counter + 1:06d}"


def generate_image( description: str, file_name: str | None = None, images: list[ str ] | None = None ) -> str:
    images_dir = os.path.join( "resources", "images" )
    os.makedirs( images_dir, exist_ok = True )

    if file_name is not None:
        ensure( "." not in file_name, "do not provide a file extension in file_name; only the base name" )
    safe_base = _to_kebab_case( file_name ) if file_name is not None else None

    client = genai.Client( api_key=os.environ.get( "GEMINI_API_KEY" ) )

    generation_config = {
         "temperature": 0.3
        ,"max_output_tokens": 65536
        ,"top_p": 0.95
        ,"thinking_level": "high"
        ,"image_config": {
             "image_size": "1K"
        }
    }

    interaction = client.interactions.create(
         model="models/gemini-3.1-flash-lite-image"
        ,input=_build_input( description, images )
        ,generation_config=generation_config
        ,response_modalities=[ "image" ]
    )

    for step in interaction.steps:
        if step.type == "model_output" and step.content:
            for part in step.content:
                if part.type == "image":
                    image_data = base64.b64decode( part.data )
                    ext = _detect_image_format( image_data )
                    file_name_final = f"{safe_base}.{ext}" if safe_base is not None else f"{_next_image_name( images_dir )}.{ext}"
                    image_path = os.path.join( images_dir, file_name_final )
                    with open( image_path, "wb" ) as f:
                        f.write( image_data )
                    return  f"image successfully created: {image_path}"

    return  ""


