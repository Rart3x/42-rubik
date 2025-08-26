import os
import sys

from Display import *
from Rubik import *
from Utils import *


def main() -> int:
    """Main function"""

    rubik = Rubik()

    if os.path.exists("./models_compressed/"):
        os.rmdir("./models_compressed")

    try:
        rubik.set_args(check_args_validity(len(sys.argv), sys.argv))
        display(rubik)

    except ValueError as e:
        print("\033[31m", e, "\033[30m")

    return 0


if __name__ == "__main__":
    main()
