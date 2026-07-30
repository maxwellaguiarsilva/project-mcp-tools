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
#   File:   general/general_lib/image_generator.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-16 12:00:00
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


def generate_image( description: str, file_name: str | None = None ) -> str:
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
        ,input=description
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


