#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#

from session.session_lib.context_tracker import get_context_usage
from tools.tool import tool


@tool( )
def context_usage( session_id: str | None = None, context_limit: int | None = None ) -> str:
    """reports how much of the model context window the current opencode chat session is using
    reads the last message token usage from the opencode database
    when session_id is omitted the most recently updated active session in the target project is used
    context_limit overrides the context limit resolved from the models.dev cache
    use the returned context_used and context_percent values in conditional instructions"""
    return  get_context_usage( session_id, context_limit )


