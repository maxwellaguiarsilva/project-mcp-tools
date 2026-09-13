#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


import re

from lib.base_verifier import base_verifier, rule
from .python_config import python_project_config


def indent_multiple_of_4( line: str ) -> bool:
    leading_spaces = len( line ) - len( line.lstrip( " " ) )
    return  leading_spaces % 4 == 0


def is_not_mixed_quotes( line: str ) -> bool:
    return  not ( "'" in line and '"' in line )


def is_not_comment_or_docstring_marker( line: str ) -> bool:
    return  not any( marker in line for marker in [ "#", '"""', "'''" ] )


def python_single_quote_filter( line: str ) -> bool:
    return  (
        indent_multiple_of_4( line )
        and is_not_mixed_quotes( line )
        and is_not_comment_or_docstring_marker( line )
    )


def python_return_yield_filter( line: str ) -> bool:
    leading_spaces = len( line ) - len( line.lstrip( " " ) )
    return  (
        leading_spaces >= 4
        and leading_spaces % 4 == 0
        and is_not_comment_or_docstring_marker( line )
    )


def python_raise_filter( line: str ) -> bool:
    leading_spaces = len( line ) - len( line.lstrip( " " ) )
    return  (
        leading_spaces % 4 == 0
        and is_not_comment_or_docstring_marker( line )
    )


def python_comment_filter( line: str ) -> bool:
    return  not any( marker in line for marker in [ '"""', "'''" ] )


class python_verifier( base_verifier ):
    @property
    def string_comment_ignore_pattern( self ):
        return  python_project_config[ "language" ][ "patterns" ][ "string-comment-ignore" ]

    @property
    def rules( self ):
        ignore = self.string_comment_ignore_pattern
        return  super( ).rules | {
             "return_spacing": rule(
                 r"^(\s*return) +(\S)"
                ,r"\1  \2"
                ,"return must be followed by exactly 2 spaces"
                ,flags = re.MULTILINE
                ,line_filter = python_return_yield_filter
            )
            ,"raise_spacing": rule(
                 r"^(\s*raise) +(\S)"
                ,r"\1   \2"
                ,"raise must be followed by exactly 3 spaces"
                ,flags = re.MULTILINE
                ,line_filter = python_raise_filter
            )
            ,"yield_spacing": rule(
                 r"^(\s*yield) +(\S)"
                ,r"\1   \2"
                ,"yield must be followed by exactly 3 spaces"
                ,flags = re.MULTILINE
                ,line_filter = python_return_yield_filter
            )
            ,"optional_to_pipe": rule(
                 r"\bOptional\[\s*(?P<type>(?:[^\[\]]|\[[^\[\]]*\])*)\s*\]"
                ,r"\g<type> | None"
                ,"""use "type | None" instead of "Optional[type]" """
                ,ignore_pattern = ignore
            )
             ,"dict_colon_spacing": rule(
                 r"""(?P<key>"[^"\\]*(?:\\.[^"\\]*)*")\s*:(?=\S)"""
                ,r"\g<key>: "
                ,"missing space after colon in dict key"
                ,ignore_pattern = ignore
            )
              ,"single_quotes": rule(
                 "'([^\x27\"\\\\]+)'"
                ,r"\g<0>"
                ,"use double quotes for string literals"
                ,line_filter = python_single_quote_filter
            )
            ,"naming_conventions": rule(
                 r"\b(class|def)\s+([a-zA-Z0-9_]*[A-Z][a-zA-Z0-9_]*)\b"
                ,r"\g<0>"
                ,"identifiers must use snake_case"
                ,ignore_pattern = ignore
            )
            ,"comment_style": rule(
                 r"^[ \t]*#\s+[A-Z][a-z]+"
                ,r"\g<0>"
                ,"comments should be lowercase"
                ,flags = re.MULTILINE
                ,line_filter = python_comment_filter
            )
            ,"comment_spacing": rule(
                 r"^[ \t]*#(?: {0,2})(?!!)(?=\S)"
                ,r"\g<0>"
                ,"""comments must use "#   " format (hash + 3 spaces)"""
                ,flags = re.MULTILINE
                ,line_filter = python_comment_filter
            )
            ,"helper_assert": rule(
                 r"\bassert\s+(.+?)\s*,\s*(.+)"
                ,r"\g<0>"
                ,"use ensure( expr, msg ) instead of assert"
                ,ignore_pattern = ignore
            )
            ,"helper_cpu_count": rule(
                 r"\bos\.cpu_count\(\s*\)"
                ,r"\g<0>"
                ,"use get_cpu_count( ) instead of os.cpu_count( )"
                ,ignore_pattern = ignore
            )
            ,"helper_json_dumps": rule(
                 r"\bjson\.dumps\("
                ,r"\g<0>"
                ,"use to_json( data ) instead of json.dumps"
                ,ignore_pattern = ignore
            )
            ,"helper_subprocess_run": rule(
                 r"\bsubprocess\.run\("
                ,r"\g<0>"
                ,"use create_process( ... ) instead of subprocess.run"
                ,ignore_pattern = ignore
            )
            ,"helper_removeprefix": rule(
                 r"\.removeprefix\("
                ,r"\g<0>"
                ,"use remove_string_prefix( text, prefix )"
                ,ignore_pattern = ignore
            )
        }


