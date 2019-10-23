#include "cmdline.hpp" // parse_command
#include "app.hpp" // handle_expr_line
#include "expr.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    std::vector<Commands::Command> commands;
    for (int i = 1; i < argc; i++) {
        try {
            auto command = parse_command(argv[i]);
            commands.push_back(command);
        } catch (const std::runtime_error & error) {
            std::cout << "! error: " << error.what() << std::endl;
        }
    }
    
    std::string line;
    while (std::getline(std::cin, line)) {
        try {
            handle_expr_line(std::cout, line, commands);
        } catch(const std::exception& e) {
            std::cout << "! error: " << e.what() << std::endl;
        }
    }
    
    return 0;
}
