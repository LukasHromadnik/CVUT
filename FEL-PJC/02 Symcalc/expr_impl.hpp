#pragma once

#include "expr.hpp"
#include "tokenizer.hpp"
#include <iosfwd>
#include <vector>
#include <math.h>

namespace exprs {

class simple_expr: public expr_base {
public:
    virtual double evaluate(const variable_map_t &variables) const override { return 0; }
    virtual expr derive(std::string const &variable) const override { return expr(); }
    virtual expr simplify() const override { return expr(); }
private:
    virtual bool equals(const expr_base& b) const override { return true; }
};

class constant_expr: public simple_expr {
public:
    constant_expr(double n) : value(n) { }
    virtual double evaluate(const variable_map_t &variables) const override { return value; }
private:
    virtual void write(std::ostream& out, WriteFormat fmt) const override;

    double value;
};

class variable_expr: public simple_expr {
public:
    variable_expr(std::string name) : name(name) { }
    virtual double evaluate(const variable_map_t &variables) const override;
private:
    virtual void write(std::ostream& out, WriteFormat fmt) const override;

    std::string name;
};

////////////////////////////////////////////// FUNCTIONS //////////////////////////////////////////////

class function_expr: public simple_expr {
public:
    function_expr() = default;
    function_expr(std::string name, expr e) : name(name), expression(e) { }
    virtual void write(std::ostream& out, WriteFormat fmt) const override;
protected:
    std::string name;
    expr expression;
};

class sin_expr: public function_expr {
public:
    sin_expr() : function_expr() { }
    sin_expr(expr e) : function_expr("sin", e) { };
    virtual double evaluate(const variable_map_t &variables) const override {
        return sin(expression->evaluate(variables));
    }
};

class cos_expr: public function_expr {
public:
    cos_expr() : function_expr() { }
    cos_expr(expr e) : function_expr("cos", e) { };
    virtual double evaluate(const variable_map_t &variables) const override {
        return cos(expression->evaluate(variables));
    }
};

class log_expr: public function_expr {
public:
    log_expr() : function_expr() { }
    log_expr(expr e) : function_expr("log", e) { };
    virtual double evaluate(const variable_map_t &variables) const override {
        double value = expression->evaluate(variables);
        if (value <= 0) {
            throw domain_exception("undefined logarithm argument");
        }
        return log(expression->evaluate(variables));
    }
};


////////////////////////////////////////////// OPERATORS //////////////////////////////////////////////

class operator_expr: public simple_expr {
public:
    operator_expr() = default;
    operator_expr(std::string sign, expr lhs, expr rhs) : sign(sign), lhs(lhs), rhs(rhs) { };
    virtual void write(std::ostream& out, WriteFormat fmt) const override;
protected:
    std::string sign;
    expr lhs;
    expr rhs;
};

class plus_expr: public operator_expr {
public:
    plus_expr() : operator_expr() { };
    plus_expr(expr lhs, expr rhs): operator_expr("+", lhs, rhs) { };
    virtual double evaluate(const variable_map_t &variables) const override {
        return lhs->evaluate(variables) + rhs->evaluate(variables);
    }
};

class minus_expr: public operator_expr {
public:
    minus_expr() : operator_expr() { };
    minus_expr(expr lhs, expr rhs): operator_expr("-", lhs, rhs) { };
    virtual double evaluate(const variable_map_t &variables) const override {
        return lhs->evaluate(variables) - rhs->evaluate(variables);
    }
};

class multiply_expr: public operator_expr {
public:
    multiply_expr() : operator_expr() { };
    multiply_expr(expr lhs, expr rhs): operator_expr("*", lhs, rhs) { };
    virtual double evaluate(const variable_map_t &variables) const override {
        return lhs->evaluate(variables) * rhs->evaluate(variables);
    }
};

class divide_expr: public operator_expr {
public:
    divide_expr() : operator_expr() { };
    divide_expr(expr lhs, expr rhs): operator_expr("/", lhs, rhs) { };
    virtual double evaluate(const variable_map_t &variables) const override {
        return lhs->evaluate(variables) / rhs->evaluate(variables);
    }
};

class pow_expr: public operator_expr {
public:
    pow_expr() : operator_expr() { }
    pow_expr(expr a, expr b) : operator_expr("^", a, b) { };
    virtual double evaluate(const variable_map_t &variables) const override {
        return pow(lhs->evaluate(variables), rhs->evaluate(variables));
    }
};

}