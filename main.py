#!/usr/bin/env python3

import argparse
import os
import sys

# from DisplayR2 import displayR2
from DisplayR3 import displayR3
from Rubik import *
from Utils import *


def main(flag, extra) -> int:
    '''Main function'''

    rubik = Rubik()

    if os.path.exists("./models_compressed/"):
        os.remove("./models_compressed/cube.bam")
        os.rmdir("./models_compressed")
    try:
        if flag == 3:
            rubik.set_args(check_args_validity(len(sys.argv), extra, flag))
        else:
            rubik.set_args(check_args_validity(len(sys.argv), extra, flag))
            
        if flag == 2:
            pass
            # displayR2(rubik.get_args())
        else:
            displayR3(rubik.get_args())
        
    except ValueError as e:
        print("\033[31m", e, "\033[30m")
    
    return 0


if __name__ == "__main__":
    '''Flag creation'''

    parser = argparse.ArgumentParser(description="Generate an interactive 3D Rubik's Cube with Ursina")
    parser.add_argument("--size", type=int, default=0, help="Specifies the size of the cube")
    parser.add_argument('extra', nargs=argparse.REMAINDER)

    args = parser.parse_args()
    
    if args.size < 2 or args.size > 3:
        print("\033[31m", "Error: size flag must be 2 or 3", "\033[30m")

    main(args.size, args.extra)