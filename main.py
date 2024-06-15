#!/usr/bin/env python3

import os
import sys

from Display import *
from Rubik import *
from Utils import *

os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = "hide"


def main() -> int:
    '''Main function'''

    rubik = Rubik()

    if os.path.exists("./models_compressed/"):
        os.remove("./models_compressed/cube.bam")
        os.rmdir("./models_compressed")

    try:
        rubik.set_args(check_args_validity(len(sys.argv), sys.argv))
        display(rubik.get_args())
    except ValueError as e:
        print("\033[31m", e, "\033[30m")


if __name__ == "__main__":
    main()
