#!/usr/bin/env python3

import argparse
import os
import sys

from DisplayR2 import *
# from DisplayR3 import *
from Rubik import *
from Utils import *


def main(flag) -> int:
    '''Main function'''

    rubik = Rubik()

    if os.path.exists("./models_compressed/"):
        os.remove("./models_compressed/cube.bam")
        os.rmdir("./models_compressed")
    
    try:
        rubik.set_args(check_args_validity(len(sys.argv), sys.argv))
        displayR2(rubik.get_args())
    except ValueError as e:
        print("\033[31m", e, "\033[30m")


if __name__ == "__main__":
    '''Flag creation'''

    parser = argparse.ArgumentParser(description="Generate an interactive 3D Rubik's Cube with Ursina")
    parser.add_argument("--size", type=int, default=0, help="Specifies the size of the cube")
    parser.add_argument('extra', nargs=argparse.REMAINDER)

    args = parser.parse_args()
    
    if args.size < 2 or args.size > 3:
        print("\033[31m", "Error: size flag must be 2 or 3", "\033[30m")

    main(args.size)