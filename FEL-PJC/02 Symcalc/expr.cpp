#include "expr.hpp"
#include "expr_impl.hpp"
#include "tokenizer.hpp"

#include <stdexcept>
#include <stack>
#include <queue>
#include <sstream>
#include <iostream>

const expr expr::ZERO = expr::number(0.0);
const expr expr::ONE = expr::number(1.0);

expr expr::number(double n) {
    return std::make_shared<exprs::constant_expr>(n);
}

expr variable(std::string name) {
    return std::make_shared<exprs::variable_expr>(name);
}

// TODO: overloaded operators +, -, *, /, functions pow, log, sin, cos,
//       expr::number, expr::variable, operator==, operator<<,
//       create_expression_tree


std::stack<Token> parse(std::string input) {
    std::istringstream stream(input);
    Tokenizer tokenizer = Tokenizer(stream);

    std::stack<Token> output_queue;
    std::stack<Token> operator_stack;

    while (true) {
        Token next = tokenizer.next();
        if (next == Token(TokenId::End)) break;

        if (next.id == TokenId::Number) {
            output_queue.push(next);
        }
        if (next.id == TokenId::Identifier) {
            operator_stack.push(next);
        }
        if (next.id == TokenId::Plus ||
            next.id == TokenId::Minus ||
            next.id == TokenId::Multiply ||
            next.id == TokenId::Divide ||
            next.id == TokenId::Power)
        {
            while (
                operator_stack.empty() == false &&
                (
                    operator_stack.top().id == TokenId::Identifier ||
                    (operator_stack.top().is_binary_op() && operator_stack.top().op_precedence() > next.op_precedence()) ||
                    (operator_stack.top().is_binary_op() && operator_stack.top().op_precedence() == next.op_precedence() && operator_stack.top().associativity() == Associativity::Left)
                ) &&
                operator_stack.top().id != TokenId::LParen)
            {
                output_queue.push(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(next);
        }
        if (next.id == TokenId::LParen) {
            operator_stack.push(next);
        }
        if (next.id == TokenId::RParen) {
            while (operator_stack.empty() == false && operator_stack.top().id != TokenId::LParen) {
                output_queue.push(operator_stack.top());
                operator_stack.pop();
            }
            if (operator_stack.empty()) {
                throw parse_error("invalid syntax");
            }
            operator_stack.pop();
        }
    }
    while (!operator_stack.empty()) {
        Token top = operator_stack.top();
        if (top.id == TokenId::LParen || top.id == TokenId::RParen) {
            throw parse_error("invalid syntax");
        }
        output_queue.push(operator_stack.top());
        operator_stack.pop();
    }

    if (output_queue.empty()) {
        throw parse_error("invalid syntax");
    }

    return output_queue;
}

struct node {
    node(Token token) : token(token) { }
    Token token;
    node *left = NULL;
    node *right = NULL;
    node *parent = NULL;

    bool is_function() const;
};

bool node::is_function() const {
    if (token.is_binary_op()) return false;
    if (token.id == TokenId::Number) return false;
    std::string id = token.identifier;
    
    return (id == "log" || id == "sin" || id == "cos");
}

void clean(node * root) {
    if (root == NULL) return;
    clean(root->left);
    clean(root->right);
    delete root;
}

void clean_tree(node * some_node) {
    node * current = some_node;
    while (current != NULL && current->parent != NULL) {
        current = current->parent;
    }
    clean(current);
}

node * build_tree(std::stack<Token> queue) {
    // std::cout << queue.top() << std::endl;
    node *root = new node(queue.top());
    queue.pop();
    node *current_node = root;

    while (queue.empty() == false) {
        // std::cout << queue.top() << std::endl;
        while (current_node != NULL && current_node->left != NULL) {
            current_node = current_node->parent;
        }

        if (current_node == NULL) {
            clean(root);
            throw parse_error("invalid syntax");
        }

        if (root->left == NULL && root->token.is_binary_op() == false && root->is_function() == false) {
            clean(root);
            throw parse_error("invalid syntax");
        }

        node *new_node = new node(queue.top());

        if (current_node != NULL && current_node->right == NULL && current_node->is_function() == false) {
            current_node->right = new_node;
            new_node->parent = current_node;
        } else if (current_node != NULL && current_node->left == NULL) {
            current_node->left = new_node;
            new_node->parent = current_node;
        }

        if (new_node->token.is_binary_op() || new_node->is_function()) {
            current_node = new_node;
        }

        queue.pop();
    }

    return root;
}

void validate_tree(node * node) {
    if (node == NULL) return;
    if (node->token.is_binary_op()) {
        if (node->left == NULL || node->right == NULL) {
            clean_tree(node);
            throw parse_error("invalid syntax");
        }
    }

    if (node->is_function()) {
        if (node->left == NULL) {
            clean_tree(node);
            throw parse_error("invalid syntax");
        }
    }

}

expr build_expression_tree(node * root) {
    if (root == NULL) return std::shared_ptr<expr_base>();
    
    expr left = build_expression_tree(root->left);
    expr right = build_expression_tree(root->right);

    Token token = root->token;

    expr expression;
    if (token.is_binary_op()) {
        switch (token.id) {
        case TokenId::Plus:
            expression = std::make_shared<exprs::plus_expr>(left, right);
            break;
        case TokenId::Minus:
            expression = std::make_shared<exprs::minus_expr>(left, right);
            break;
        case TokenId::Multiply:
            expression = std::make_shared<exprs::multiply_expr>(left, right);
            break;
        case TokenId::Divide:
            expression = std::make_shared<exprs::divide_expr>(left, right);
            break;
        case TokenId::Power:
            expression = std::make_shared<exprs::pow_expr>(left, right);
            break;
        default:
            throw std::logic_error("undefined operator");
        }
    } else if (token.id == TokenId::Number) {
        expression = std::make_shared<exprs::constant_expr>(token.number);
    } else if (token.id == TokenId::Identifier) {
        if (token.identifier == "sin") {
            expression = std::make_shared<exprs::sin_expr>(left);
        } else if (token.identifier == "cos") {
            expression = std::make_shared<exprs::cos_expr>(left);
        } else if (token.identifier == "log") {
            expression = std::make_shared<exprs::log_expr>(left);
        } else {
            expression = std::make_shared<exprs::variable_expr>(token.identifier);
        }
    }
    return expression;
    
}

expr create_expression_tree(const std::string& expression) {
    std::stack<Token> queue = parse(expression);
    node * root = build_tree(queue);
    validate_tree(root);
    expr result = build_expression_tree(root);
    clean(root);

    return result;
}

bool operator==(const expr &a, const expr &b) {
    throw std::logic_error("not implemented");
}

std::ostream& operator<<(std::ostream &os, const expr &e) {
    e->write(os, expr_base::WriteFormat::Prefix);
    return os;
}

expr operator+(expr a, expr b) {
    return std::make_shared<exprs::plus_expr>(a, b);
}

expr operator-(expr a, expr b) {
    return std::make_shared<exprs::minus_expr>(a, b);
}

expr operator*(expr a, expr b) {
    return std::make_shared<exprs::multiply_expr>(a, b);
}

expr operator/(expr a, expr b) {
    return std::make_shared<exprs::divide_expr>(a, b);
}

expr pow(expr m, expr e) {
    return std::make_shared<exprs::pow_expr>(m, e);
}

expr sin(expr e) {
    return std::make_shared<exprs::sin_expr>(e);
}

expr cos(expr e) {
    return std::make_shared<exprs::cos_expr>(e);
}

expr log(expr e) {
    return std::make_shared<exprs::log_expr>(e);
}
