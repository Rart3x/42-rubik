#include "includes/Rubik.hpp"
#include "includes/Utils.hpp"

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

void    test() {
    printf("TEST");
}

int main(int ac, char **av) {

    Rubik rubik;

    std::vector<std::string> args;

    try {
        rubik.setArgs(check_args_validity(ac, av));
    }
    catch (const std::exception& e) {
        std::cerr << "\033[31m" << e.what() << "\033[30m" << std::endl;
    }
}