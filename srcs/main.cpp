#include "includes/Rubik.hpp"
#include "includes/Utils.hpp"

int main(int ac, char **av) {

    Rubik rubik;

    std::vector<std::string> args;
    (void)ac;
    (void)av;

    try {
        rubik.setArgs(check_args_validity(ac, av));
    }
    catch (const std::exception& e) {
        std::cerr << "\033[31m" << e.what() << "\033[30m" << std::endl;
    }
}

// EMSCRIPTEN_BINDINGS(module)
// {
//     emscripten::function("args", &check_args_validity);
// };