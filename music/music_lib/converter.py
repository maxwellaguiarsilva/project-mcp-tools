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
#   File:   project-mcp-tools/music/music_lib/converter.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-26 00:00:00
#   


import re

from sak.common import to_json

key_map = {
     "a": ( "c", 0 )
    ,"s": ( "d", 0 )
    ,"d": ( "e", 0 )
    ,"f": ( "f", 0 )
    ,"g": ( "g", 0 )
    ,"h": ( "a", 0 )
    ,"j": ( "b", 0 )
    ,"k": ( "c", 1 )
}


def convert_keyboard_notes( input_text: str ) -> str:
    notes = [ ]
    current_base_octave = 4

    lines = input_text.splitlines( )
    for line in lines:
        line_str = line.strip( )
        if not line_str:
            continue

        match = re.match( r"^([+-]\d+)", line_str )
        if match:
            modifier_str = match.group( 1 )
            current_base_octave += int( modifier_str )
            line_str = line_str[ len( modifier_str ) : ].strip( )

        mode = None
        i = 0
        while i < len( line_str ):
            char = line_str[ i ]

            if char == "(":
                mode = "whole"
                i += 1
                continue
            elif char == ")":
                mode = None
                i += 1
                continue
            elif char == "[":
                mode = "half"
                i += 1
                continue
            elif char == "]":
                mode = None
                i += 1
                continue
            elif char == "_":
                mode = None if mode == "sixteenth" else "sixteenth"
                i += 1
                continue

            char_lower = char.lower( )
            if char_lower in key_map or char_lower == "r":
                if char_lower == "r":
                    note_name = "rest"
                else:
                    pitch_name, octave_offset = key_map[ char_lower ]
                    note_name = f"{ pitch_name }{ current_base_octave + octave_offset }"

                if mode == "whole":
                    duration = 4.0
                elif mode == "half":
                    duration = 2.0
                elif mode == "sixteenth":
                    duration = 0.25
                elif char.isupper( ):
                    duration = 1.0
                else:
                    duration = 0.5

                notes.append( [ note_name, duration ] )

            i += 1

    return  to_json( notes )


