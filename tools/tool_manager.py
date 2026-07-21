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
#   File:   tools/tool_manager.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-15 14:00:00
#   


import json
import os
import sys
import inspect
from typing import Any, Dict

from fastapi import FastAPI
import uvicorn
from fastmcp import FastMCP as _FastMCP

from tools.tool import tool_info, contract_success, contract_error
from tools.folder_scanner import scan_folder
from sak.common import ensure, ensure_list, to_json, from_json, create_process, value_or, get_process_text


class tool_manager:
    def __init__( self, name: str, path_manager ):
        self._name = name
        self._path_manager = path_manager
        self._registry: dict = { }
        self._registry[ "list_tools" ] = tool_info( "list_tools", self.list_tools, self._run_in_subprocess )

    def add_tool_folder( self, folder_name: str | list[ str ], flg_use_prefix: bool = True ):
        for name in ensure_list( folder_name, str ):
            for prefixed_name, func, description in scan_folder( name, self._path_manager.project_root, flg_use_prefix ):
                self._registry[ prefixed_name ] = tool_info( prefixed_name, func, self._run_in_subprocess )

    def execute_tool( self, tool_name, kwargs ):
        tool_info_obj = self._registry.get( tool_name.replace( "-", "_" ) )
        if tool_info_obj is None:
            raise   Exception( f"""tool "{tool_name}" not found""" )
        func = tool_info_obj.function
        bound = inspect.signature( func ).bind( **kwargs )
        bound.apply_defaults( )
        return  func( *bound.args, **bound.kwargs )

    def _run_in_subprocess( self, tool_name, kwargs ):
        result = create_process(
             [ sys.executable, "-m", "main", tool_name ]
            ,input = to_json( kwargs )
            ,check = False
            ,env = os.environ | self._path_manager.to_env( ) | { "PYTHONPATH": self._path_manager.project_root + os.pathsep + os.environ.get( "PYTHONPATH", "" ) }
            ,cwd = self._path_manager.target_root
        )
        output_text = get_process_text( result ) 
        return  contract_success( output_text ) if result.returncode == 0 else contract_error( output_text )

    def register_module_tools( self, module_globals: dict ):
        for value in module_globals.values( ):
            if callable( value ) and getattr( value, "__is_tool__", False ):
                name = value.__tool_name__
                self._registry[ name ] = tool_info( name, value, self._run_in_subprocess )

    def run_cli( self, args: list ):
        ensure( len( args ) > 0, "no tool name provided" )
        tool_name = args[ 0 ].replace( "-", "_" )
        tool_info_obj = self._registry.get( tool_name )
        ensure( tool_info_obj is not None, f"unknown tool: {args[ 0 ]}" )

        parser = tool_info_obj.build_argparse_parser( )
        print( to_json( self._run_in_subprocess( tool_name, vars( parser.parse_args( args[ 1: ] ) ) ) ) )

    def run_mcp( self ):
        sys.argv = [ sys.argv[ 0 ] ]
        self._mcp = _FastMCP( self._name )
        for name, tool_info_obj in self._registry.items( ):
            self._add_mcp_tool( name, tool_info_obj )
        self._mcp.run( )

    def run_api( self, host: str = "0.0.0.0", port: int = 8000 ):
        app = FastAPI( title = self._name )

        for name, tool_info_obj in self._registry.items( ):
            endpoint = tool_info_obj.build_api_endpoint( )
            app.post( f"/tools/{name}", response_model = Dict[ str, Any ] )( endpoint )

        uvicorn.run( app, host = host, port = port )

    def _add_mcp_tool( self, tool_name: str, tool_info_obj: tool_info ):
        wrapper = tool_info_obj.build_mcp_wrapper( )
        self._mcp.add_tool( wrapper )

    def list_tools( self ):
        """returns a list of available tools"""
        return  [ info.to_dict for info in self._registry.values( ) ]


