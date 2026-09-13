#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


import os

from sak.common import to_json
from tools.tool import tool
from cpp.cpp_lib.llm_verifier_core import run_llm_verifier, write_markdown_report


#   @tool( )
#   intentionally disabled at the user's request on 2026-08-02 11:58:52 AM -03
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


