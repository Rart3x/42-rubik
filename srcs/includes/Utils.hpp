#pragma once

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <set>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

bool                        is_in_set(char* arg);
bool                        check_splitted_args(std::vector<std::string> args);

std::vector<std::string>    check_args_validity(int ac, char **av);