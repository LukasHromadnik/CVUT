#include "tiny-04.hpp"

#include <stack>
#include <iostream>

using std::cout;
using std::endl;

std::vector<double> BST::preorder_elements() const {
    if (!m_root) {
        return {};
    }

    std::vector<double> values;
    std::stack<node*> nodes;
    nodes.push(m_root);

    while (!nodes.empty()) {
        auto current = nodes.top(); nodes.pop();

        values.push_back(current->value);
        if (current->right) {
            nodes.push(current->right);
        }
        if (current->left) {
            nodes.push(current->left);
        }
    }

    return values;
}

void BST::add(double value) {
    auto position = &m_root;
    while (*position != nullptr) {
        if (value < (*position)->value) {
            position = &(*position)->left;
        } else if (value > (*position)->value) {
            position = &(*position)->right;
        } else {
            // This value is already present...
            return;
        }
    }
    *position = new node{ nullptr, nullptr, value };
}

bool BST::contains(double value) const {
    auto next = m_root;
    while (next != nullptr) {
        if (value < next->value) {
            next = next->left;
        } else if (value > next->value) {
            next = next->right;
        } else {
            // Found the right value
            return true;
        }
    }
    // Walked the tree without finding the value
    return false;
}

BST::BST(BST const& rhs) {
    for (double value : rhs.preorder_elements()) {
        this->add(value);
    }
}

BST& BST::operator=(BST const& rhs) {
    if (m_root) {
        std::stack<node*> nodes;
        nodes.push(m_root);

        while (!nodes.empty()) {
            auto current = nodes.top(); nodes.pop();
            if (current->left) {
                nodes.push(current->left);
            }
            if (current->right) {
                nodes.push(current->right);
            }
            delete current;
        }
    }
    m_root = nullptr;
    for (double value : rhs.preorder_elements()) {
        this->add(value);
    }
    return *this;
}

BST::BST(BST&& rhs) {
    for (double value : rhs.preorder_elements()) {
        this->add(value);
    }
}

BST& BST::operator=(BST&& rhs) {
    if (m_root) {
        std::stack<node*> nodes;
        nodes.push(m_root);

        while (!nodes.empty()) {
            auto current = nodes.top(); nodes.pop();
            if (current->left) {
                nodes.push(current->left);
            }
            if (current->right) {
                nodes.push(current->right);
            }
            delete current;
        }
    }
    m_root = nullptr;
    for (double value : rhs.preorder_elements()) {
        this->add(value);
    }
    return *this;
}

BST::~BST() {
    if (m_root) {
        std::stack<node*> nodes;
        nodes.push(m_root);

        while (!nodes.empty()) {
            auto current = nodes.top(); nodes.pop();
            if (current->left) {
                nodes.push(current->left);
            }
            if (current->right) {
                nodes.push(current->right);
            }
            delete current;
        }
    }
}

