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
#   File:   tools/tool.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-06-06 13:42:23
#   


import argparse
import inspect
from dataclasses import dataclass
from typing import Any, Callable, Dict

from pydantic import create_model

from sak.common import set_attributes, to_dict


def contract_success( data ):
    return  { "status": "success", "data": data }


def contract_error( message ):
    return  { "status": "error", "error": message }


def tool( name: str | None = None, description: str | None = None ):
    """standalone decorator that marks a function as a tool (no side effects)"""
    def decorator( func: Callable ):
        func.__is_tool__ = True
        func.__tool_name__ = name or func.__name__
        func.__tool_description__ = description or func.__doc__
        return  func
    return  decorator


@dataclass
class parameter_info:
    name: str
    annotation: type | None
    default: Any
    required: bool

    @property
    def to_dict( self ):
        d = { "name": self.name, "type": self.annotation.__name__ }
        if self.required:
            d[ "required" ] = True
        else:
            d.update( default = self.default, required = False )
        return  d


class tool_info:
    def __init__( self, name: str, func: Callable, runner: Callable | None = None ):
        sig = inspect.signature( func )
        self.name = name
        self.description = getattr( func, "__tool_description__", None ) or func.__doc__ or ""
        self.function = func
        self.runner = runner
        self.parameters = [
            parameter_info(
                 name       = p_name
                ,annotation = p.annotation if p.annotation != inspect.Parameter.empty else str
                ,default    = p.default if p.default != inspect.Parameter.empty else None
                ,required   = p.default == inspect.Parameter.empty
            )
            for p_name, p in sig.parameters.items( )
        ]
        self.signature = sig
        self.annotations = getattr( func, "__annotations__", { } )

    @property
    def to_dict( self ):
        return  to_dict( self, [ "name", "description", "parameters" ] )

    def build_argparse_parser( self ) -> argparse.ArgumentParser:
        parser = argparse.ArgumentParser( description = self.description or self.name )
        for param in self.parameters:
            is_required = param.required
            is_bool = param.annotation is bool
            valid_type = param.annotation if param.annotation in ( int, float, str ) else str
            kwargs = (
                  ( { "type": valid_type } if not is_bool else { } )
                | ( { "default": param.default } if not is_required and not is_bool else { } )
                | ( { "action": argparse.BooleanOptionalAction, "default": False if is_required else param.default }
                    if is_bool else { }
                )
            )
            parser.add_argument(
                 f"""--{ param.name.replace( "_", "-" ) }"""
                ,required=is_required
                ,**kwargs # type: ignore
            )
        return  parser

    def build_input_model( self ):
        fields = {
            param.name: ( ( param.annotation, ... ) if param.required else ( param.annotation, param.default ) )
            for param in self.parameters
        }
        return  create_model( f"{self.name}_input", **fields )

    def build_api_endpoint( self ):
        input_model = self.build_input_model( )
        async def endpoint( input_data: input_model ):  # type: ignore[valid-type]
            return  self.runner( self.name, input_data.model_dump( ) )
        endpoint.__name__ = self.name
        endpoint.__doc__ = self.description
        return  endpoint

    def build_mcp_wrapper( self ) -> Dict[ str, Any ]:
        def wrapper( *args, **kwargs ) -> Dict[ str, Any ]:
            return  self.runner( self.name, kwargs )
        set_attributes( wrapper, {
             "__name__": self.name
            ,"__doc__": self.description
            ,"__signature__": self.signature.replace( return_annotation = Dict[ str, Any ] )
            ,"__annotations__": self.annotations | { "return": Dict[ str, Any ] }
        } )
        return  wrapper


