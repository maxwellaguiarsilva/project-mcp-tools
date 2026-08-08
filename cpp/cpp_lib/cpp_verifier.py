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
#   File:   cpp/cpp_lib/cpp_verifier.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-01-22 19:12:03
#   


#


import re

from lib.base_verifier import base_verifier, re_line_break, rule
from lib.project_file import line_break, triple_line_break
from .cpp_project_config import cpp_project_config


re_consecutive_newlines = rf"(?:{re_line_break}){{4,}}"

re_return_spacing = r"(?P<ws>[ \t])return\b(?![ \t]*;)[ \t]*"
sub_return_spacing = r"\g<ws>return\g<ws>"

re_include_no_empty = rf"(^#include\s+.*){re_line_break}(?:[ \t]*{re_line_break})+(?=#include\s+.*)"
sub_include_no_empty = r"\1" + line_break

re_include_before = rf"^((?!#include).+)(?:{re_line_break})+(#include\s+.*)"
re_include_after = rf"(^#include\s+.*)(?:{re_line_break})+((?!#include).+)"
sub_before_nl3_after = r"\1" + triple_line_break + r"\2"


def cpp_return_filter( line: str ) -> bool:
    return  not any( marker in line for marker in [ "//", "/*", "*/", '"' ] )


class cpp_verifier( base_verifier ):
    @property
    def string_comment_ignore_pattern( self ):
        return  cpp_project_config[ "language" ][ "patterns" ][ "string_comment_ignore" ]

    @property
    def rules( self ):
        ignore = self.string_comment_ignore_pattern
        return  super( ).rules | {
             "consecutive_newlines": rule(
                 re_consecutive_newlines
                ,triple_line_break
                ,"too many consecutive empty lines (maximum 2 allowed)"
                ,ignore_pattern = ignore
            )
            ,"return_spacing": rule(
                 re_return_spacing
                ,sub_return_spacing
                ,"return must be followed by exactly one space or tab (matching the preceding indentation character)"
                ,ignore_pattern = ignore
                ,line_filter = cpp_return_filter
            )
            ,"include_no_empty": rule(
                  re_include_no_empty
                 ,sub_include_no_empty
                 ,"include directives must not be separated by empty lines"
                 ,flags = re.MULTILINE
             )
            ,"include_before": rule(
                  re_include_before
                 ,sub_before_nl3_after
                 ,"there must be exactly two empty lines before the first include"
                 ,flags = re.MULTILINE
             )
            ,"include_after": rule(
                  re_include_after
                 ,sub_before_nl3_after
                 ,"there must be exactly two empty lines after the last include"
                 ,flags = re.MULTILINE
             )
        }


