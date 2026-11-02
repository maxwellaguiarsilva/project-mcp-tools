#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


from tools.tool import tool
from git.git_lib.upload import perform_upload

@tool( )
def quick_upload( message: str, project_name: str | None = None ) -> str:
    """performs a quick git upload: pull, add all, commit with message, and push
by default the upload runs on the current target project
when project_name is given, the upload runs on the sibling project with that name, resolved next to the target root
this tool is intended for simple, non-conflicting changes to increase agility
inspect git status and git diff directly to produce the commit message using the Conventional Commits standard ( do not use git log; history must not influence the commit message decision, only status and diff )
mandatory: the message must be in en-us"""
    return  perform_upload( message, project_name )


