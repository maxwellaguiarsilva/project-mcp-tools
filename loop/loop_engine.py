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
#   File:   loop/loop_engine.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-21 19:10:00
#   


import argparse

from loop.loop_lib.engine_core import run_loop, loop_config


def main( target_root: str, args: list[ str ] ) -> None:
    """entry point for the loop mode
    parses loop-specific arguments and starts the engine"""
    config = _parse_args( target_root, args )
    result = run_loop( config )
    print( f"[loop] result: success={result.success} iterations={result.iterations}" )


def _parse_args( target_root: str, args: list[ str ] ) -> loop_config:
    parser = argparse.ArgumentParser( description = "opencode programmer/police loop engine" )
    parser.add_argument( "--task", default = None, help = "initial task prompt for the programmer" )
    parser.add_argument( "--spec-dir", default = "docs/agent", help = "directory with specification docs" )
    parser.add_argument( "--max-iterations", type = int, default = 20, help = "safety limit" )
    parser.add_argument( "--programmer-timeout", type = int, default = 600, help = "programmer subprocess timeout in seconds" )
    parser.add_argument( "--police-timeout", type = int, default = 300, help = "police subprocess timeout in seconds" )
    parser.add_argument( "--programmer-model", default = None, help = "model for programmer agent" )
    parser.add_argument( "--police-model", default = None, help = "model for police agent" )
    parser.add_argument( "--log-file", default = "loop-engine.log", help = "iteration log path" )
    parser.add_argument( "--dry-run", action = "store_true", help = "print planned commands without executing" )
    parsed = parser.parse_args( args )
    return  loop_config(
         target_root          = target_root
        ,task                 = parsed.task or ""
        ,spec_dir             = parsed.spec_dir
        ,max_iterations       = parsed.max_iterations
        ,programmer_timeout   = parsed.programmer_timeout
        ,police_timeout       = parsed.police_timeout
        ,programmer_model     = parsed.programmer_model
        ,police_model         = parsed.police_model
        ,log_file             = parsed.log_file
        ,flg_dry_run          = parsed.dry_run
    )
