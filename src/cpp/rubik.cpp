/*!
 *  @file rubik.cpp
 *  @brief Implementation file for the rubik main program.
 *
 *  This file serves as the entry point for the rubik program.
 */

#include "rubik.hpp"
#include <iostream>
#include <string>
#include <vector>


/**
 *  @brief Validates the input string for rubik moves.
 *
 *  This function checks if the input string contains only valid characters
 *  representing rubik moves and their modifiers.
 *
 *  @param input The input string to validate.
 *  @return true if the input is valid, false otherwise.
 */
bool isValidInput(const std::string& input) {
    const std::string validChars = "FRUBLD";
    for(size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        if(c != ' ' && c != '\'' && c != '2' && validChars.find(c) == std::string::npos) {
            std::cerr << "Invalid character in input: " << c << std::endl;
            return false;
        }
		if (c == '\'' || c == '2') {
			if (i == 0 || validChars.find(input[i - 1]) == std::string::npos) {
                std::cerr << "Invalid modifier position: " << c << std::endl;
                return false;
			}
		}
    }
    return true;
}

/**
 *  @brief Main function of the rubik program.
 *
 *  This function initializes and starts the rubik program.
 *
 *  @param argc Argument count.
 *  @param argv Argument vector.
 *  @return Exit status.
 */
int main(int argc, char** argv) {
    std::cout << "Hello, Rubik!" << std::endl;
	std::string entry;

    bool continuous = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
		if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: rubik [-c|--continuous] [-h|--help] [mixing_sequence]\n"
                      << "  -c, --continuous   Process multiple inputs until 'QUIT' is received.\n"
                      << "  -h, --help         Display this help message.\n"
					  << "  mixing_sequence    A sequence of moves to solve (e.g., \"U R U' L' D2 F B'\").\n";
            return 0;
        } else if (arg == "-c" || arg == "--continuous") {
            continuous = true;
        } else if (entry.empty()) {
            entry = arg;
        }
    }

    while (true) {
		if (entry.empty()) {
            if (!std::getline(std::cin, entry))
            	break;
        }
        if (entry == "QUIT")
            break;
		if (!isValidInput(entry)) {
            if (!continuous)
                break;
            else {
				entry.clear();
                continue;
			}
        }
        // auto moves = Solve(entry);
		std::vector moves = {"U", "R", "U'", "L'", "D", "F", "B'"}; // Placeholder for actual moves
        if (moves.empty()) {
			std::cout << "SOLVED\n";
        } else {
            for (size_t i=0; i < moves.size(); ++i) {
				if(i)
					std::cout<<' ';
				std::cout<<moves[i];
			}
            std::cout << "\n";
        }
        std::cout << std::flush;
        if (!continuous)
            break;
		entry.clear();
    }
    return 0;
}