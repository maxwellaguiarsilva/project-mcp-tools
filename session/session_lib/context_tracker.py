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
#   File:   session/session_lib/context_tracker.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-08-08 14:07:00
#   


#

import json
import os
import sqlite3
from datetime import datetime

from sak.common import ensure, to_json


def _data_dir( ):
    xdg = os.environ.get( "XDG_DATA_HOME" ) or os.path.join( os.path.expanduser( "~" ), ".local", "share" )
    return  os.path.join( xdg, "opencode" )


def _cache_dir( ):
    xdg = os.environ.get( "XDG_CACHE_HOME" ) or os.path.join( os.path.expanduser( "~" ), ".cache" )
    return  os.path.join( xdg, "opencode" )


def _database_path( ):
    override = os.environ.get( "OPENCODE_DB" )
    if override:
        return  override if os.path.isabs( override ) else os.path.join( _data_dir( ), override )
    return  os.path.join( _data_dir( ), "opencode.db" )


def _target_root( ):
    return  os.environ.get( "PROJECT_MCP_TARGET_ROOT", os.getcwd( ) )


def _current_session( cursor, directory: str ):
    primary = cursor.execute(
        "SELECT * FROM session WHERE directory = ? AND time_archived IS NULL AND parent_id IS NULL"
        " ORDER BY time_updated DESC LIMIT 1",
        ( directory, ),
    ).fetchone( )
    return  primary if primary else cursor.execute(
        "SELECT * FROM session WHERE directory = ? AND time_archived IS NULL ORDER BY time_updated DESC LIMIT 1",
        ( directory, ),
    ).fetchone( )


def _session_by_id( cursor, session_id: str ):
    return  cursor.execute(
        "SELECT * FROM session WHERE id = ? LIMIT 1",
        ( session_id, ),
    ).fetchone( )


def _last_usage( cursor, session_id: str ):
    rows = cursor.execute(
        "SELECT data FROM message WHERE session_id = ? ORDER BY time_created DESC, id DESC LIMIT 20",
        ( session_id, ),
    ).fetchall( )
    for ( data, ) in rows:
        tokens = json.loads( data ).get( "tokens" )
        if tokens and ( tokens.get( "total" ) or tokens.get( "input" ) or tokens.get( "cache", { } ).get( "read" ) ):
            return  tokens
    return  None


def _as_int( value ) -> int | None:
    try:
        return  int( value ) if value is not None else None
    except ( TypeError, ValueError ):
        return  None


def _model_context_limit( provider_id: str, model_id: str ) -> int | None:
    models_path = os.path.join( _cache_dir( ), "models.json" )
    if not os.path.isfile( models_path ):
        return  None
    try:
        with open( models_path ) as f:
            models = json.load( f )
        limit = models.get( provider_id, { } ).get( "models", { } ).get( model_id, { } ).get( "limit", { } ).get( "context" )
        return  int( limit ) if isinstance( limit, int ) else None
    except ( OSError, ValueError, TypeError ):
        return  None


def get_context_usage( session_id: str | None, context_limit: int | None ) -> str:
    db_path = _database_path( )
    ensure( os.path.isfile( db_path ), f"opencode database not found at: {db_path}" )
    con = sqlite3.connect( f"file:{db_path}?mode=ro", uri = True )
    try:
        cursor = con.cursor( )
        session = _session_by_id( cursor, session_id ) if session_id else _current_session( cursor, _target_root( ) )
        ensure( session, "opencode session not found in the target project" )
        columns = [ column[ 0 ] for column in cursor.description ]
        session_data = dict( zip( columns, session ) )
        tokens = _last_usage( cursor, session_data[ "id" ] )

        model = json.loads( session_data[ "model" ] or "{ }" ) if session_data.get( "model" ) else { }
        provider_id = model.get( "providerID" )
        model_id = model.get( "id" )
        limit = _as_int( context_limit ) or _model_context_limit( provider_id, model_id )

        if not tokens:
            return  to_json( {
                 "session_id": session_data[ "id" ]
                ,"directory": session_data[ "directory" ]
                ,"context_used": 0
                ,"context_percent": 0
                ,"message": "less than 1k tokens: the session just started and no token usage is recorded yet. "
                            "This is expected, not a failure. Call again later in the session."
            } )

        input_tokens = tokens.get( "input", 0 )
        output_tokens = tokens.get( "output", 0 )
        reasoning_tokens = tokens.get( "reasoning", 0 )
        cache_read = tokens.get( "cache", { } ).get( "read", 0 )
        cache_write = tokens.get( "cache", { } ).get( "write", 0 )
        context_used = input_tokens + cache_read + cache_write
        total = tokens.get( "total" ) or ( context_used + output_tokens + reasoning_tokens )
    finally:
        con.close( )

    result = {
         "session_id": session_data[ "id" ]
        ,"directory": session_data[ "directory" ]
        ,"title": session_data[ "title" ]
        ,"model": f"{ provider_id }/{ model_id }" if provider_id and model_id else None
        ,"context_limit": limit
        ,"context_used": context_used
        ,"context_percent": round( 100.0 * context_used / limit, 1 ) if limit else None
        ,"last_request": {
             "input": input_tokens
            ,"output": output_tokens
            ,"reasoning": reasoning_tokens
            ,"cache_read": cache_read
            ,"cache_write": cache_write
            ,"total": total
        }
        ,"session_totals": {
             "input": session_data[ "tokens_input" ]
            ,"output": session_data[ "tokens_output" ]
            ,"reasoning": session_data[ "tokens_reasoning" ]
            ,"cache_read": session_data[ "tokens_cache_read" ]
            ,"cache_write": session_data[ "tokens_cache_write" ]
        }
        ,"session_cost": session_data[ "cost" ]
        ,"updated_at": datetime.fromtimestamp( session_data[ "time_updated" ] / 1000 ).isoformat( )
    }
    return  to_json( result )


