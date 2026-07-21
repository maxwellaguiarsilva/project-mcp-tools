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
#   File:   cpp/cpp_lib/llm_rules.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-19 00:00:00
#   


#

import os
import re

from dataclasses import dataclass
from pathlib import Path


@dataclass
class llm_rule:
    rule_id: str
    name: str
    description: str
    complexity: str
    examples: str


_rules_md_path = str(
     Path( __file__ ).resolve( ).parent.parent.parent
    /"docs"/"agent"/"style-guide"/"cpp.md"
)

_meta_pattern = re.compile( r"<!--\s*llm-rule:\s*id=(\w+),\s*complexity=(\w+)\s*-->" )

_rules_cache: list[ llm_rule ] | None = None


def _resolve_markdown_path( ) -> str:
    return  os.environ.get( "CPP_LLM_RULES_MD", _rules_md_path )


def _extract_description( text: str ) -> str:
    desc = re.sub( r"```.*?```", "", text, flags = re.DOTALL )
    desc = re.sub( r"\n{3,}", "\n\n", desc )
    return  desc.strip( )


def _extract_examples( text: str ) -> str:
    blocks = re.findall( r"```cpp\n(.*?)```", text, re.DOTALL )
    return  "\n".join( block.strip( ) for block in blocks if block.strip( ) )


def _parse_rules_from_markdown( md_path: str ) -> list[ llm_rule ]:
    rules: list[ llm_rule ] = [ ]

    try:
        with open( md_path, "r", encoding = "utf-8" ) as file_handle:
            content = file_handle.read( )
    except ( FileNotFoundError, PermissionError, UnicodeDecodeError ):
        return  rules

    sections = re.split( r"\n(?=##\s)", content )

    for section in sections[ 1: ]:
        heading = section.split( "\n", 1 )[ 0 ]
        if "[llm]" not in heading:
            continue

        name = re.sub( r"\s*`?\[[^\]]*\]`?", "", heading )
        name = name.lstrip( "#" ).strip( )

        meta_match = _meta_pattern.search( section )
        if not meta_match:
            continue

        rule_id = meta_match.group( 1 )
        complexity = meta_match.group( 2 )

        body = section[ : meta_match.start( ) ] + section[ meta_match.end( ) : ]
        body_lines = body.split( "\n", 1 )
        body = body_lines[ 1 ] if len( body_lines ) > 1 else ""

        description = _extract_description( body )
        examples = _extract_examples( body )

        rules.append( llm_rule(
             rule_id     = rule_id
            ,name        = name
            ,description = description
            ,complexity  = complexity
            ,examples    = examples
        ) )

    return  rules


def _load_rules( ) -> list[ llm_rule ]:
    global _rules_cache
    if _rules_cache is None:
        _rules_cache = _parse_rules_from_markdown( _resolve_markdown_path( ) )
    return  _rules_cache


def get_all_rules( ) -> list[ llm_rule ]:
    return  _load_rules( )


def get_rules_by_complexity( complexity: str ) -> list[ llm_rule ]:
    return  [ rule for rule in _load_rules( ) if rule.complexity == complexity ]


def get_rules_by_ids( rule_ids: list[ str ] ) -> list[ llm_rule ]:
    return  [ rule for rule in _load_rules( ) if rule.rule_id in rule_ids ]


def get_rule_by_id( rule_id: str ) -> llm_rule | None:
    for rule in _load_rules( ):
        if rule.rule_id == rule_id:
            return  rule
    return  None


def build_verification_prompt( rule: llm_rule, file_path: str, file_content: str ) -> str:
    return  (
         f"""you are verifying that a c++ file follows a specific coding rule from the project style guide"""
         f"""\n\nrule name: {rule.name}"""
         f"""\n\nrule definition:"""
         f"""\n{rule.description}"""
         f"""\n\nexamples:"""
         f"""\n{rule.examples}"""
         f"""\n\nfile: {file_path}"""
         f"""\n\nfile content:"""
         f"""\n```cpp"""
         f"""\n{file_content}"""
         f"""\n```"""
         f"""\n\ninstructions:"""
         f"""\n1. scan the entire file for violations of this rule"""
         f"""\n2. if the file has zero violations, respond with exactly: ok"""
         f"""\n3. if violations are found, respond in this format for each:"""
         f"""\n   violation: line <number>: <description of the violation and what should be changed>"""
         f"""\n4. do not report violations in the license header block at the top of the file"""
         f"""\n5. do not report violations in code that is commented out"""
         f"""\n6. do not fabricate or reach for issues — only report clear, unambiguous violations"""
         f"""\n7. if the rule does not apply to this file type (e.g., a header-only rule checked against a .cpp file), respond with: ok"""
    )


def build_verification_prompt_all_rules( rules: list[ llm_rule ], file_path: str, file_content: str ) -> str:
    rules_text = ""
    for rule in rules:
        rules_text += f"""\n\n---\n\nrule: {rule.name} (id: {rule.rule_id}, complexity: {rule.complexity})\n\n{rule.description}"""
        if rule.examples:
            rules_text += f"""\n\nexamples:\n{rule.examples}"""
    return  (
         f"""you are verifying that a c++ file follows the project's coding style guide"""
         f"""\n\nthe guide defines the following rules. for each violation, include the rule id in the output."""
         f"""{rules_text}"""
         f"""\n\n---"""
         f"""\n\nfile: {file_path}"""
         f"""\n\nfile content:"""
         f"""\n```cpp"""
         f"""\n{file_content}"""
         f"""\n```"""
         f"""\n\ninstructions:"""
         f"""\n1. scan the entire file for violations of ALL rules listed above"""
         f"""\n2. if the file has zero violations against all rules, respond with exactly: ok"""
         f"""\n3. if violations are found, respond in this format for each:"""
         f"""\n   violation: <rule_id> line <number>: <description of the violation and what should be changed>"""
         f"""\n4. do not report violations in the license header block at the top of the file"""
         f"""\n5. do not report violations in code that is commented out"""
         f"""\n6. do not fabricate or reach for issues — only report clear, unambiguous violations"""
         f"""\n7. if a rule does not apply to this file type, skip it for this file"""
         f"""\n8. report multiple violations of the same rule if they occur on different lines"""
    )
