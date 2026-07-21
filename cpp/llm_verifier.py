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
#   File:   cpp/llm_verifier.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-19 00:00:00


import os

from sak.common import to_json
from tools.tool import tool
from cpp.cpp_lib.llm_verifier_core import run_llm_verifier, write_markdown_report


@tool( )
def llm_verifier(
     files:     list[ str ] | None = None
    ,rules:     list[ str ] | None = None
    ,model:     str                   = "deepseek/deepseek-v4-flash"
    ,output_md: str                   = "violations.md"
    ) -> str:
    """verifies c++ files against llm-level coding rules using opencode --auto
all rules are checked together in a single run per file, with violations tagged by rule id
returns a list of potential violations that should be audited by the main session
if files is not provided, all .cpp/.hpp files in the project are checked
if rules is not provided, all llm rules are checked
the tool runs opencode in --auto mode with automatic tool approval for each check
use output_md to write a markdown report (e.g., "violations.md")"""
    target_root = os.environ.get( "PROJECT_MCP_TARGET_ROOT", os.getcwd( ) )
    result = run_llm_verifier( files, rules, target_root, model )
    result_dict = result.to_dict
    if output_md:
        result_dict[ "output_md" ] = write_markdown_report( result, output_md )
    return  to_json( result_dict )
