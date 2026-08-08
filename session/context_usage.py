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
#   File:   session/context_usage.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-08-08 14:07:00
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


