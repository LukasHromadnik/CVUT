#include "expr.hpp"
#include "cmdline.hpp"

#include <iostream>

using namespace std;

void process_expr(std::ostream &os, expr initial_expr, vector<Commands::Command> const &commands) {
    expr e = std::move(initial_expr);
    for (const auto &cmd : commands) {
        using namespace Commands;
        cmd.match(
            [&](Commands::Derive const &derive) {
                // TODO
            },
            [&](Commands::Simplify const &) {
                // TODO
            },
            [&](Commands::Evaluate const &evaluate) {
                try {
                    os << e->evaluate(evaluate.variables);
                    os << std::endl;
                } catch (const unbound_variable_exception & e) {
                    os << "! error: " << e.what() << endl;
                }
            },
            [&](Commands::Print const &p) {
                os << e;
                os << std::endl;
            }
        );
    }
}

void handle_expr_line(std::ostream &os, std::string const &line, vector<Commands::Command> const &commands) {
    expr initial_expr = create_expression_tree(line);
    process_expr(os, initial_expr, commands);
}
