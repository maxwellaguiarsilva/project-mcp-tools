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
#   File:   cpp/cpp_lib/cpp_project_config.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-01-22 19:12:03
#   


#


import copy

from sak.common import deep_update
from lib.project_config import project_config


cpp_project_config = {
    #   compiler information
    "compiler": {
         "executable": "g++"            #   options: "g++", "clang++" or full path
        ,"standard": "c++26"            #   examples: "c++17", "c++20", "c++23", "c++26"
        ,"use-64-bits": True            #   abstraction for -m64 (64-bit)
    }

    #   build rules (how to compile)
    ,"build_behavior": {
        #   options: "none" (-O0), "balanced" (-O2), "aggressive" (-O3), "debug" (-Og)
         "experimental-library": False  #   enables -fexperimental-library
    }

    #   quality control (warning and analysis flags)
    ,"quality_control": {
        #   options: "minimal", "high" (-Wall -Wextra), "pedantic"
         "warning-level": "high"
        ,"treat-warnings-as-errors": True #   -Werror
        ,"stop-on-first-error": True      #   -Wfatal-errors
        ,"static_analysis": {
            "enabled": True
            #   cppcheck suppressions, appended as --suppress=<item> when present
            ,"suppress": [
                 "*:*/glad/*"
                ,"functionStatic"
                ,"unusedFunction"
            ]
            #   cppcheck file filter, appended as --file-filter=<item> when present
            ,"file-filter": [ ]
            #   cppcheck force: check all configurations instead of the default limit
            ,"force": False
        }
    }

    #   external dependencies (no -l or -I prefixes)
    ,"dependencies": {
         "libraries": [ ]               #   example: ["ncurses", "pthread"]
        ,"include-dirs": [ ]            #   additional paths for header search
        ,"library-dirs": [ ]            #   additional paths for library search
    }

    #   language information
    ,"language": {
         "source-extension": "cpp"
        ,"header-extension": "hpp"
        ,"comment-string":  "//\t"
        ,"patterns": {
            "string-comment-ignore": r"//.*|/\*[\s\S]*?\*/|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'"
        }
    }

    #   folder organization
    ,"paths": {
         "source": "source"             #   project source directory containing .cpp files
        ,"include": "include"           #   project include directory containing .hpp files
        ,"tests": "tests"               #   test source directory containing .cpp files
        ,"adhoc": "tests/adhoc"         #   adhoc tests directory
        ,"build": "build"               #   build directory for object files (.o)
        ,"output": "dist"               #   output directory for the final binaries
    }
}


cpp_release_config  =   {
    
    #   compiler information
    "compiler": {
        "extra-compile-flags": [
             "-ffunction-sections"      #   place each function into its own section
            ,"-fdata-sections"          #   place each data item into its own section
            ,"-flto"                    #   enable link time optimization
        ]
        ,"extra-link-flags": [
            "-flto"                     #   enable link time optimization during linking
        ]
        ,"linker-direct-options": [
             "--as-needed"              #   only link libraries that satisfy undefined symbols
            ,"--gc-sections"            #   remove unused sections (dead code elimination)
        ]
    }
    
    #   build rules (how to compile)
    ,"build_behavior": {
        #   options: "none" (-O0), "balanced" (-O2), "aggressive" (-O3), "debug" (-Og)
         "optimization": "balanced"
        ,"debug-symbols": False         #   generates symbols for gdb (-g)
        ,"generate-dependencies": False #   generates .d files (intelligent recompilation)
    }
}


#   the cpp config performs a deep_update on the generic config
cpp_project_config = deep_update( deep_update( copy.deepcopy( project_config ), cpp_project_config ), cpp_release_config )


