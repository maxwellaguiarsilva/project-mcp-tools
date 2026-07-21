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
#   File:   loop/loop_lib/session_monitor.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-21 19:00:00
#   


import os
import sqlite3
from dataclasses import dataclass

from sak.common import to_dict


@dataclass
class session_info:
    session_id: str = ""
    title: str = ""
    agent: str = ""
    model: str = ""
    cost: float = 0.0
    tokens_input: int = 0
    tokens_output: int = 0
    tokens_reasoning: int = 0
    tokens_cache_read: int = 0
    tokens_cache_write: int = 0
    time_created: int = 0
    time_updated: int = 0
    message_count: int = 0
    found: bool = False

    @property
    def to_dict( self ):
        return  to_dict( self, [
            "session_id", "title", "agent", "model", "cost",
            "tokens_input", "tokens_output", "tokens_reasoning",
            "tokens_cache_read", "tokens_cache_write",
            "time_created", "time_updated", "message_count", "found",
        ] )


def get_db_path( ) -> str:
    return  os.path.expanduser( "~/.local/share/opencode/opencode.db" )


def query_session( session_id: str ) -> session_info:
    """queries opencode.db for session diagnostics
    returns a session_info with found false if the db or session is missing"""
    db_path = get_db_path( )
    if not os.path.exists( db_path ):
        return  session_info( session_id = session_id )
    conn = sqlite3.connect( db_path )
    try:
        conn.row_factory = sqlite3.Row
        row = conn.execute(
            "SELECT id, title, agent, model, cost, tokens_input, tokens_output,"
            " tokens_reasoning, tokens_cache_read, tokens_cache_write,"
            " time_created, time_updated FROM session WHERE id = ?",
            ( session_id, )
        ).fetchone( )
        if row is None:
            return  session_info( session_id = session_id )
        msg_count = conn.execute(
            "SELECT COUNT(*) FROM message WHERE session_id = ?",
            ( session_id, )
        ).fetchone( )[ 0 ]
        return  session_info(
             session_id          = row[ "id" ]
            ,title               = row[ "title" ] or ""
            ,agent               = row[ "agent" ] or ""
            ,model               = row[ "model" ] or ""
            ,cost                = row[ "cost" ] or 0.0
            ,tokens_input        = row[ "tokens_input" ] or 0
            ,tokens_output       = row[ "tokens_output" ] or 0
            ,tokens_reasoning    = row[ "tokens_reasoning" ] or 0
            ,tokens_cache_read   = row[ "tokens_cache_read" ] or 0
            ,tokens_cache_write  = row[ "tokens_cache_write" ] or 0
            ,time_created        = row[ "time_created" ] or 0
            ,time_updated        = row[ "time_updated" ] or 0
            ,message_count       = msg_count
            ,found               = True
        )
    finally:
        conn.close( )


def query_latest_session( ) -> session_info | None:
    """returns the most recently updated session from opencode.db
    returns none if the db is missing or empty"""
    db_path = get_db_path( )
    if not os.path.exists( db_path ):
        return  None
    conn = sqlite3.connect( db_path )
    try:
        conn.row_factory = sqlite3.Row
        row = conn.execute(
            "SELECT id FROM session ORDER BY time_updated DESC LIMIT 1"
        ).fetchone( )
        if row is None:
            return  None
        return  query_session( row[ "id" ] )
    finally:
        conn.close( )
