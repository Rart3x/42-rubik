import os
import random


def check_args_validity(ac, av):
    """CheckArgs validity"""
    
    if ac > 2:
        raise ValueError("Error: Invalid number of arguments")
    if ac < 2:
        return

    if not is_in_set(av[1]):
        raise ValueError("Error: Invalid caracter in arguments")

    args = av[1].split()

    if not check_splitted_args(args):
        raise ValueError("Error: Invalid pattern in arguments")
    if not is_spaced(args):
        raise ValueError("Error: Invalid spacing in arguments")
#     if not check_textures():
#         raise ValueError("Error: Missing textures")

    return args


def check_textures():
    """CheckTextures function"""

    textures = {
        "textures/"
        "textures/cube.png",
        "textures/cube.obj"
    }

    for texture in textures:
        if not os.path.exists(texture):
            return False


def check_splitted_args(args):
    """CheckSplit function"""

    letters = {
        'F', 'R', 'U', 'B', 'L', 'D'
    } | {
        'f', 'r', 'u', 'b', 'l', 'd'
    }
    numbers = {'2'}

    for arg in args:
        prev_char = None
        for i, char in enumerate(arg):
            if i == 0 and (char in numbers or char == "'"):
                return False
            if (
                i != 0
                and (char in numbers or char == "'")
                and arg[i - 1] not in letters
            ):
                return False
            if prev_char in letters and char in letters:
                return False
            prev_char = char

    return True


def decompose_arr_args(arr: [], dest: []):
    """Decompose array arguments in N arguments and append them on dest"""

    for args in arr:
        dest.append(args)


def expand_double_inputs(args):
    """Recreate args [] and cloning X2 inputs by X * 2 input"""

    modified_args = []

    for i in args:
        if len(i) > 1 and i[1].isdigit():
            modified_args.extend([i[0]] * 2)
        else:
            modified_args.append(i)
    return modified_args


def generate_input(i):
    """Generate I input to apply to the Rubik's Cube"""

    inputs_chars = [
        "f", "r", "u", "b", "l", "d", "e", "m", "s",
        "f2", "r2", "u2", "b2", "l2", "d2", "e2", "m2", "s2",
        "f'", "r'", "u'", "b'", "l'", "d'", "e'", "m'", "s'"
    ]

    random_chars = []
    
    for _ in range(i):
        index = random.randint(0, len(inputs_chars) - 1)
        char = inputs_chars[index]
        random_chars.append(char)

    return random_chars


def is_in_set(arg):
    """IsInSet function"""

    set_chars = {
        'F', 'R', 'U', 'B', 'L', 'D', '2', "'", ' '
    } | {
        'f', 'r', 'u', 'b', 'l', 'd'
    }

    for char in arg:
        if char not in set_chars:
            return False
    return True


def is_spaced(args):
    """IsSpaced function"""

    for arg in args:
        if len(arg) > 2:
            return False
    return True


def reverse_seq(seq: []):
    """Reverse a sequence of movements"""

    reverse = []

    for arg in seq:
        if len(arg) == 1:
            reverse.append(arg + "'")
        else:
            reverse.append(arg[0])

    return reverse
