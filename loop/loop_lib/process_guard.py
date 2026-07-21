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
#   File:   loop/loop_lib/process_guard.py
#   Author: Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   
#   Created on 2026-07-21 18:45:00
#   


import time
import subprocess
from dataclasses import dataclass

from sak.common import to_dict


@dataclass
class guarded_result:
    success: bool
    returncode: int
    stdout: str
    stderr: str
    timed_out: bool
    duration_s: float

    @property
    def to_dict( self ):
        return  to_dict( self, [ "success", "returncode", "timed_out", "duration_s" ] )


def run_guarded(
     command:    list[ str ]
    ,timeout:    int
    ,env:        dict | None = None
    ,cwd:        str | None = None
    ) -> guarded_result:
    """runs a subprocess with a hard timeout
    on timeout the process is killed and partial output is captured
    this is the primary hangout defense
    uses popen directly instead of create_process because create_process
    cannot capture partial output on timeout"""
    start = time.monotonic( )
    proc = subprocess.Popen(
         command
        ,stdout = subprocess.PIPE
        ,stderr = subprocess.PIPE
        ,text   = True
        ,env    = env
        ,cwd    = cwd
    )
    try:
        stdout, stderr = proc.communicate( timeout = timeout )
        duration = time.monotonic( ) - start
        return  guarded_result(
             success    = proc.returncode == 0
            ,returncode = proc.returncode
            ,stdout     = stdout or ""
            ,stderr     = stderr or ""
            ,timed_out  = False
            ,duration_s = round( duration, 2 )
        )
    except subprocess.TimeoutExpired:
        proc.kill( )
        stdout, stderr = proc.communicate( )
        duration = time.monotonic( ) - start
        return  guarded_result(
             success    = False
            ,returncode = -1
            ,stdout     = stdout or ""
            ,stderr     = stderr or ""
            ,timed_out  = True
            ,duration_s = round( duration, 2 )
        )
