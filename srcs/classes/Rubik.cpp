#include "../includes/Rubik.hpp"

Rubik::Rubik() {}
Rubik::~Rubik() {}

std::vector<std::string>    Rubik::getArgs() const {
    return this->args;
}

void    Rubik::setArgs(const std::vector<std::string>& args) {
    this->args = args;
}