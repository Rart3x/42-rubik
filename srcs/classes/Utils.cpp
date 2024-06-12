#include "../includes/Utils.hpp"

bool     is_in_set(char *arg) {

    std::set<char> set = {'F', 'R', 'U', 'B', 'L', 'D', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '\'', ' '};

    for (size_t i = 0; i < strlen(arg); i++) {
        if (set.find(arg[i]) == set.end())
            return false;
    }
    return true;
}

bool     check_splitted_args(std::vector<std::string> args) {

    std::vector<std::string>::iterator it;

    std::set<char> letters = {'F', 'R', 'U', 'B', 'L', 'D'};
    std::set<char> numbers = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    for (it = args.begin(); it < args.end(); it++) {
        for (size_t i = 0; i < (*it).size(); i++) {
            if (i == 0 && (numbers.find((*it)[i]) != numbers.end() || (*it)[i] == '\'')) {
                return false;
            }
            if (i != 0 && (numbers.find((*it)[i]) != numbers.end() || (*it)[i] == '\'')) {
                if (letters.find((*it)[i - 1]) == letters.end()) {
                    return false;
                }
            }
        }
    }
    return true;
}

std::vector<std::string>    check_args_validity(int ac, char **av) {

    std::string tmp;
    std::vector<std::string> args;
    std::istringstream iss(av[1]);

    if (ac != 2)
        throw std::invalid_argument("Invalid number of arguments");
    if (!is_in_set(av[1]))
        throw std::invalid_argument("Invalid caracter in argument");

    while (std::getline(iss, tmp, ' '))
        args.push_back(tmp);

    if (!check_splitted_args(args))
        throw std::invalid_argument("Invalid pattern in argument");

    return args;
}