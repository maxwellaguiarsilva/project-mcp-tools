#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


import copy

from sak.common import deep_update
from lib.project_config import project_config


python_project_config = {
    "language": {
         "extension": "py"
        ,"comment-string": "#   "
        ,"patterns": {
            "string-comment-ignore": r"#.*|\"\"\"[\s\S]*?\"\"\"|'''[\s\S]*?'''|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'"
        }
    }
}

python_project_config = deep_update( copy.deepcopy( project_config ), python_project_config )


