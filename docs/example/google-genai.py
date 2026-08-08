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
#   File:   docs/example/google-genai.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-07 21:37:12
#   


import base64
import os

from google import genai
from IPython.display import Image, display


client = genai.Client(
     api_key = os.environ.get( "GEMINI_API_KEY" )
)

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
     model = "models/gemini-3.1-flash-lite-image"
    ,input = "Present a clear, 45° top-down isometric miniature 3D cartoon scene of London, featuring its most iconic landmarks and architectural elements. Use soft, refined textures with realistic PBR materials and gentle, lifelike lighting and shadows. Integrate the current weather conditions directly into the city environment to create an immersive atmospheric mood. Use a clean, minimalistic composition with a soft, solid-colored background."
    ,generation_config = generation_config
    ,response_modalities = [ "image" ]
)

for step in interaction.steps:
    if step.type == "model_output" and step.content:
        for part in step.content:
            if part.type == "text":
                print( part.text )
            elif part.type == "image":
                display( Image( data = base64.b64decode( part.data ) ) )


