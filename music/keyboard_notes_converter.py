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
#   File:   project-mcp-tools/music/keyboard_notes_converter.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-26 00:00:00
#   


from tools.tool import tool
from music.music_lib.converter import convert_keyboard_notes


@tool( )
def keyboard_notes_converter( input_text: str ) -> str:
    """converts user keyboard note transcriptions into json note arrays for game audio
    parses shorthand key notation into standard note and duration pairs

    notation rules:
    asdfghjk maps to c4, d4, e4, f4, g4, a4, b4, c5
    uppercase letters represent 1 beat quarter notes
    lowercase letters represent 0.5 beat eighth notes
    enclosing in () represents 4 beats whole notes
    enclosing in [] represents 2 beats half notes
    enclosing in _ represents 0.25 beat sixteenth notes
    +1 or -1 at start of line adjusts base octave"""
    return  convert_keyboard_notes( input_text )


