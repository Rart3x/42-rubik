import random


def check_args_validity(ac, extra, flag):
    '''CheckArgs validity'''
    
    print
    if ac > 4:
        raise ValueError("Error: Invalid number of arguments")
    if ac < 4:
        return

    if not is_in_set(extra):
        raise ValueError("Error: Invalid caracter in arguments")

    args = extra.split()

    if not check_splitted_args(args):
        raise ValueError("Error: Invalid pattern in arguments")
    if not is_spaced(args):
        raise ValueError("Error: Invalid spacing in arguments")

    return args


def check_splitted_args(args):
    '''CheckSplitted function'''

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


def generate_input(i):
    '''Generate i input to apply to the Rubik's Cube'''

    inputs_chars = [
        "F", "R", "U", "B", "L", "D", "E", "M", "S",
        "F2", "R2", "U2", "B2", "L2", "D2" "E2", "M2", "S2",
        "F'", "R'", "U'", "B'", "L'", "D'", "E'", "M'", "S'"
    ]
    
    random_chars = []
    
    for _ in range(i):
        index = random.randint(0, len(inputs_chars) - 1)
        char = inputs_chars[index]
        random_chars.append(char)

    return random_chars


def is_in_set(arg):
    '''IsInSet function'''

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
    '''IsSpaced function'''

    for arg in args:
        if len(arg) > 2:
            return False
    return True
