#!/usr/bin/python3

#   
#   SPDX-FileCopyrightText: 2026 Maxwell Aguiar Silva <maxwellaguiarsilva@gmail.com>
#   SPDX-License-Identifier: GPL-3.0-or-later
#   


#

import random

from sak.common import ensure
from tools.tool import tool


@tool( )
def get_random_number( start: int = 1, end: int = 100 ) -> str:
    """returns a random number between start and end"""
    ensure( end > start, "invalid range" )
    return  str( random.randint( start, end ) )


