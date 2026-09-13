#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
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


