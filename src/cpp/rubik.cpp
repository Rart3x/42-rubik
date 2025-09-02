/*!
 *  @file rubik.cpp
 *  @brief Implementation file for the rubik main program.
 *
 *  This file serves as the entry point for the rubik program.
 */

#include "rubik.hpp"

/**
 *  @brief Main function of the rubik program.
 *
 *  This function initializes and starts the rubik program.
 *
 *  @param argc Argument count.
 *  @param argv Argument vector.
 *  @return Exit status.
 *
 */
int main(int argc, char** argv) {
    std::cout << "Hello, Rubik!" << std::endl;
    if (argc >= 2) {
        std::cout << argv[1] << std::endl;
    }
    return 0;
}