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
#   File:   tools/folder_scanner.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-06-05 17:39:57
#   


import os
import sys
import importlib

from sak.common import to_json


def scan_folder( folder_name, project_root, flg_use_prefix = True ):
    base_dir = str( project_root )
    folder_path = os.path.join( base_dir, folder_name )

    if not os.path.isdir( folder_path ):
        return

    if base_dir not in sys.path:
        sys.path.insert( 0, base_dir )

    for entry in os.listdir( folder_path ):
        file_path = os.path.join( folder_path, entry )
        if not os.path.isfile( file_path ) or not entry.endswith( ".py" ):
            continue
        module_name = f"{ folder_name }.{ entry[ : -3 ] }"

        try:
            module = importlib.import_module( module_name )
        except Exception as e:
            print(
                to_json( { "status": "error", "error": f"error loading { module_name }: { e }" } ),
                file = sys.stderr
            )
            continue

        for attr_name in dir( module ):
            attr = getattr( module, attr_name )
            if (    not callable( attr )
                or  not getattr( attr, "__is_tool__", False )
                or  getattr( attr, "__module__", "" ) != module_name
            ):
                continue
            tool_name = attr.__tool_name__ if not flg_use_prefix else f"{ folder_name }_{ attr.__tool_name__ }"
            yield   tool_name, attr, attr.__tool_description__


