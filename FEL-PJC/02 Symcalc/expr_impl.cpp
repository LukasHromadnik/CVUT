#include "expr_impl.hpp"

#include <iostream>
#include <cmath>
#include <limits>

namespace exprs {

//////////////////// OPERATOR ////////////////////

void operator_expr::write(std::ostream &out, WriteFormat fmt) const {
    out << "(" << sign << " " << lhs << " " << rhs << ")";
}

//////////////////// FUNCTION ////////////////////

void function_expr::write(std::ostream &out, WriteFormat fmt) const {
    out << "(" << name << " " << expression << ")";
}

//////////////////// CONSTANT ////////////////////

void constant_expr::write(std::ostream& out, WriteFormat fmt) const {
    out << value;
}

//////////////////// VARIBLE ////////////////////

void variable_expr::write(std::ostream& out, WriteFormat fmt) const {
    out << name;
}

double variable_expr::evaluate(const variable_map_t &variables) const {
    for (auto item : variables) {
        if (item.first == name) {
            return item.second;
        }
    }
    throw unbound_variable_exception("unbounded variable");
}

} // namespace exprs
