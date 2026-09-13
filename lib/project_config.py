#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#


from sak.common import get_cpu_count, deep_update, get_git_config, ensure

project_config = {
    "max-threads": get_cpu_count( )
    #   folder organization
    ,"paths": {
         "tools": "."                  #   project tools directory
        ,"docs": "docs"                #   project documentation directory
        ,"templates": "docs/templates" #   project templates directory
    }
    ,"author": {
         "name": get_git_config( "user.name" ) or ensure( False, "failed to retrieve user.name from git" )
        ,"email": get_git_config( "user.email" ) or ensure( False, "failed to retrieve user.email from git" )
    }
    ,"locale": {
        "datetime-format": "%Y-%m-%d %H:%M:%S"
    }
    ,"system": {
        "pattern": {
            "line-break": "\n"
        }
    }

}


