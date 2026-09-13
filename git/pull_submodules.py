#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


from tools.tool import tool
from git.git_lib.submodules import pull_all

@tool( )
def pull_submodules( ) -> str:
    """updates every submodule of the target project to the latest commit of its remote branch
each submodule must have a clean git status, otherwise the tool fails without touching anything
the updated pointers are left in the working tree so the owner decides when to commit the bump"""
    return  pull_all( )


