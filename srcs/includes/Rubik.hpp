#pragma once

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <set>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#include "Utils.hpp"

class Rubik {
    public:
        Rubik();
        ~Rubik();

        std::vector<std::string>    getArgs(void) const;

        void                        setArgs(const std::vector<std::string>& args);


    private:
        std::vector<std::string> args;
};