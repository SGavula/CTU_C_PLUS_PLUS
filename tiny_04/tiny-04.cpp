#include "tiny-04.hpp"

#include <stack>

using namespace std;

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
    if(!rhs.m_root) {
        m_root = nullptr;
        return;
    }

    // Copy root
    m_root = new node{nullptr, nullptr, rhs.m_root->value};

    stack<pair<node*, node*>> nodes_stack;
    nodes_stack.push({m_root, rhs.m_root});

    while(!nodes_stack.empty()) {
        auto nodes_pair = nodes_stack.top();
        nodes_stack.pop();

        node* new_node = nodes_pair.first;
        node* old_node = nodes_pair.second;

        if(old_node->left) {
            new_node->left = new node{nullptr, nullptr, old_node->left->value};
            nodes_stack.push({new_node->left, old_node->left});
        }

        if(old_node->right) {
            new_node->right = new node{nullptr, nullptr, old_node->right->value};
            nodes_stack.push({new_node->right, old_node->right});
        }
    }
}

BST& BST::operator=(BST const& rhs) {
    if(this != &rhs) {
        // Clean the existing tree
        if(m_root) {
            stack<node*> nodes_stack;
            nodes_stack.push(m_root);

            while(!nodes_stack.empty()) {
                node* current = nodes_stack.top();
                nodes_stack.pop();
                if(current->left) {
                    nodes_stack.push(current->left);
                }
                if(current->right) {
                    nodes_stack.push(current->right);
                }
                delete current;
            }
        }

        // Add copy to three
        m_root = nullptr;
        if(rhs.m_root) {
            // Copy root
            m_root = new node{nullptr, nullptr, rhs.m_root->value};

            stack<pair<node*, node*>> nodes_stack;
            nodes_stack.push({m_root, rhs.m_root});

            while(!nodes_stack.empty()) {
                auto nodes_pair = nodes_stack.top();
                nodes_stack.pop();

                node* new_node = nodes_pair.first;
                node* old_node = nodes_pair.second;

                if(old_node->left) {
                    new_node->left = new node{nullptr, nullptr, old_node->left->value};
                    nodes_stack.push({new_node->left, old_node->left});
                }

                if(old_node->right) {
                    new_node->right = new node{nullptr, nullptr, old_node->right->value};
                    nodes_stack.push({new_node->right, old_node->right});
                }
            }
        }
    }

    return *this;
}

BST::BST(BST&& rhs) {
    if(!rhs.m_root) {
        m_root = nullptr;
        return;
    }

    // Copy root
    m_root = rhs.m_root;

    // Clean original tree
    rhs.m_root = nullptr;
}

BST& BST::operator=(BST&& rhs) {
    if(this != &rhs) {
        // Clean the existing tree
        if(m_root) {
            stack<node*> nodes_stack;
            nodes_stack.push(m_root);

            while(!nodes_stack.empty()) {
                node* current = nodes_stack.top();
                nodes_stack.pop();
                if(current->left) {
                    nodes_stack.push(current->left);
                }
                if(current->right) {
                    nodes_stack.push(current->right);
                }
                delete current;
            }
        }

        // Copy root
        m_root = rhs.m_root;

        // Clean original tree
        rhs.m_root = nullptr;
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

