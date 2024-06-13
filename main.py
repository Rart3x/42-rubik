#!/usr/bin/env python3

import os
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = "hide"

import sys

from Display import *
from Rubik   import *
from Utils   import *

def main() -> int:
    '''Main function'''

    rubik = Rubik()

    try:
        rubik.set_args(check_args_validity(len(sys.argv), sys.argv))
        display()
    except ValueError as e:
        print("\033[31m", e, "\033[30m")


if __name__ == "__main__":
    main()
