#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


import re
from dataclasses import dataclass

from lib.project_file import line_break, triple_line_break


re_line_break = re.escape( line_break )
re_space = rf"[ \t{re_line_break}]"

re_open_space = rf"(?P<char>[(\[{{])(?!{re_space})"
re_close_space = rf"(?<!{re_space})(?P<char>[)\]\}}])"


@dataclass
class rule:
    pattern: str
    replacement: str
    message: str
    ignore_pattern: str = None
    flags: int = 0
    line_filter: object = None


class base_verifier:
    def __init__( self, model, files = [ ], flg_auto_fix = False ):
        self.model = model
        self.files = files
        self.flg_auto_fix = flg_auto_fix

    @property
    def string_comment_ignore_pattern( self ):
        raise   NotImplementedError( "string_comment_ignore_pattern must be implemented in subclasses" )

    @property
    def rules( self ):
        ignore = self.string_comment_ignore_pattern
        return  {
             "open_space": rule( re_open_space, r"\g<char> ", r"missing space after '\g<char>'", ignore_pattern = ignore )
            ,"close_space": rule( re_close_space, r" \g<char>", r"missing space before '\g<char>'", ignore_pattern = ignore )
        }

    def run( self ) -> str:
        results = [ ]
        rules = self.rules
        
        for item in self.model.get_files( self.files ):
            violations = [ ]
            header = item.expected_license_header.strip( line_break )
            body = item.body
            line_offset = ( header + triple_line_break ).count( line_break )

            flg_ignore_helpers = "#ignore-helpers-rule" in body
            for key, r in rules.items( ):
                if flg_ignore_helpers and key.startswith( "helper_" ):
                    continue
                pattern = f"({r.ignore_pattern})|({r.pattern})" if r.ignore_pattern else r.pattern
                
                def sub_func( match ):
                    if r.ignore_pattern and match.group( 1 ):
                        return  match.group( 0 )
                    
                    if r.line_filter:
                        match_start = match.start( )
                        line_start = body.rfind( line_break, 0, match_start ) + 1
                        line_end = body.find( line_break, match_start )
                        if line_end == -1:
                            line_end = len( body )
                        line_content = body[ line_start:line_end ]
                        
                        if not r.line_filter( line_content ):
                            return  match.group( 0 )
                    
                    replacement = match.expand( r.replacement )
                    if match.group( 0 ) != replacement or r.replacement == r"\g<0>":
                        line_no = line_offset + body.count( line_break, 0, match.start( ) ) + 1
                        kind = "warning-only" if r.replacement == r"\g<0>" else "autofix-done"
                        violations.append( f"line {line_no}: [{kind}] {match.expand( r.message )}" )
                    return  replacement
                
                body = re.sub( pattern, sub_func, body, flags = r.flags )

            new_content = f"{header}{triple_line_break}{body.strip( line_break )}{triple_line_break}"
            
            if item.content != new_content:
                if self.flg_auto_fix:
                    item.write( new_content )
            
            if violations:
                results.append( f"file: {item.path}\n" + "".join( f"  {v}\n" for v in violations ) )
        
        res = line_break.join( results ).strip( )

        return  res or f"no formatting violations found in the provided files"


