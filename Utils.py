def is_in_set(arg):
    '''IsInSet function'''

    set_chars = {'F', 'R', 'U', 'B', 'L', 'D', '2', "'", ' '}
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


def check_splitted_args(args):
    '''CheckSplitted function'''

    letters = {'F', 'R', 'U', 'B', 'L', 'D'}
    numbers = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}

    for arg in args:
        prev_char = None
        for i, char in enumerate(arg):
            if i == 0 and (char in numbers or char == "'"):
                return False
            if i!= 0 and (char in numbers or char == "'") and arg[i - 1] not in letters:
                return False
            if prev_char in letters and char in letters:
                return False
            prev_char = char

    return True


def check_args_validity(ac, av):
    '''CheckArgs validity'''

    if ac!= 2:
        raise ValueError("Error: Invalid number of arguments")
    
    if not is_in_set(av[1]):
        raise ValueError("Error: Invalid caracter in arguments")

    args = av[1].split()

    if not check_splitted_args(args):
        raise ValueError("Error: Invalid pattern in arguments")
    if not is_spaced(args):
        raise ValueError("Error: Invalid spacing in arguments")

    return args
