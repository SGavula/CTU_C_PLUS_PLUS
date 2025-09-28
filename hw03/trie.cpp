#include "trie.hpp"

#include <utility>
#include <algorithm>
#include <cassert>
#include <queue>
#include <stack>
#include <functional>
#include <iostream>
#include <thread>  // For std::this_thread::sleep_for
#include <chrono>


trie::trie() {
    m_root = new trie_node();
    m_size = 0;
}

trie::trie(const std::vector<std::string>& strings) : trie() {
    for(auto str : strings) {
        insert(str);
    }
}

void copy_nodes(trie_node* new_node, trie_node* original_node) {
    for(size_t i = 0; i < num_chars; i++) {
        if(original_node->children[i] == nullptr) {
            continue;
        }

        new_node->children[i] = new trie_node(*original_node->children[i]);
        new_node->children[i]->parent = new_node;
        copy_nodes(new_node->children[i], original_node->children[i]);
    }
}

trie::trie(const trie& rhs) {
    if(rhs.m_root) {
        m_root = new trie_node(*rhs.m_root);
        m_size = rhs.m_size;

        copy_nodes(m_root, rhs.m_root);
    }
}

trie::trie(trie&& rhs) {
    if(rhs.m_root) {
        m_root = rhs.m_root;
        m_size = rhs.m_size;
        rhs.m_root = nullptr;
        rhs.m_size = 0;
    }
}

void delete_node(trie_node* node) {
    for(auto child : node->children) {
        if(child == nullptr) {
            continue;
        }
        delete_node(child);
    }
    delete node;
}

trie::~trie() {
    if(m_root) {
        delete_node(m_root);
        m_root = nullptr;
        m_size = 0;
    }
}

trie& trie::operator=(const trie& rhs) {
    if(this == &rhs) {
        return *this;
    }

    if(rhs.m_root) {
        delete_node(m_root);
        m_root = new trie_node(*rhs.m_root);
        m_size = rhs.m_size;

        copy_nodes(m_root, rhs.m_root);
    }else {
        m_root = nullptr;
        m_size = 0;
    }

    return *this;
}

trie& trie::operator=(trie&& rhs) {
    if(this == &rhs) {
        return *this;
    }

    if(m_root) {
        delete_node(m_root);
    }

    if(rhs.m_root) {
        m_root = rhs.m_root;
        m_size = rhs.m_size;
        rhs.m_root = nullptr;
        rhs.m_size = 0;
    }else {
        m_root = nullptr;
        m_size = 0;
    }

    return *this;
}

bool trie::erase(const std::string& str) {
    if(str.empty()) {
        if(m_root->is_terminal) {
            m_root->is_terminal = false;
            m_size--;
            return true;
        }
        return false;
    }

    trie_node* curr_node = m_root;
    int len = str.size();
    int str_idx = 0;
    std::vector<trie_node*> path;

    while(true) {
        int ascii_idx = (int)str[str_idx];
        trie_node* child = curr_node->children[ascii_idx];
        path.push_back(child);

        if(child == nullptr) {
            // If child == nullptr that means that we want to erase some string that is not on the trie
            return false;
        }

        curr_node = child;
        str_idx++;

        if(str_idx == len) {
            if(curr_node->is_terminal) {
                // Current node is terminal and the trie contains string and we erase it
                curr_node->is_terminal = false;
                m_size--;
                for(int i = path.size() - 1; i >= 0; i--) {
                    trie_node* path_node = path[i];
                    if(path_node->is_terminal) {
                        break;
                    }

                    bool has_children = false;
                    for (trie_node* child : path_node->children) {
                        if (child != nullptr) {
                            has_children = true;
                            break;
                        }
                    }
                    if(has_children) {
                        break;
                    }
                    int child_idx = (int)path_node->payload;
                    if(path_node->parent) {
                        path_node->parent->children[child_idx] = nullptr;
                    }
                    path.pop_back();
                    delete path_node;
                }
                return true;
            }
            // Current node is not terminal so the trie does not contain the string
            return false;
        }
    }
}

bool trie::insert(const std::string& str) {
    if(str.empty()) {
        if(m_root->is_terminal) {
           // Empty string is in trie
            return false;
        }
        m_root->is_terminal = true;
        m_size++;
        return true;
    }

    trie_node* curr_node = m_root;
    int len = str.size();
    int str_idx = 0;

    while(true) {
        int ascii_idx = (int)str[str_idx];
        trie_node* child = curr_node->children[ascii_idx];

        if(child == nullptr) {
            // Add new node to trie
            curr_node->children[ascii_idx] = new trie_node();
            curr_node->children[ascii_idx]->parent = curr_node;
            curr_node->children[ascii_idx]->payload = str[str_idx];
        }

        curr_node = curr_node->children[ascii_idx];
        str_idx++;

        if(str_idx == len) {
            if(curr_node->is_terminal) {
                return false;
            }
            curr_node->is_terminal = true;
            m_size++;
            return true;
        }
    }
}

bool trie::contains(const std::string& str) const {
    if(str.empty()) {
        if(m_root->is_terminal) {
            return true;
        }
        return false;
    }

    trie_node* curr_node = m_root;
    int len = str.size();
    int str_idx = 0;

    while(true) {
        int ascii_idx = (int)str[str_idx];
        trie_node* child = curr_node->children[ascii_idx];

        if(child == nullptr) {
            // If child == nullptr that means that we want to erase some string that is not on the trie
            return false;
        }

        curr_node = child;
        str_idx++;

        if(str_idx == len) {
            if(curr_node->is_terminal) {
                // Current node is terminal and the trie contains string and we erase it
                return true;
            }
            // Current node is not terminal so the trie does not contain the string
            return false;
        }
    }
}

size_t trie::size() const {
    return m_size;
}

bool trie::empty() const {
    if(m_size == 0) {
        return true;
    }

    return false;
}

// Stage 02
trie::const_iterator::const_iterator(const trie_node* node) : current_node(node) {}

trie::const_iterator trie::begin() const {
    const_iterator it = const_iterator(m_root);
    if(!m_root->is_terminal) {
        ++it;
    }
    return it;
}

trie::const_iterator trie::end() const {
    return const_iterator(nullptr);
}

trie::const_iterator& trie::const_iterator::operator++() {
    if (!current_node) {
        return *this;
    }

    // First, travel down to find a terminal child
    auto findTerminalChild = [](trie_node* curr_node) -> trie_node* {
        std::function<trie_node *(trie_node *)> dfs = [&](trie_node *node) -> trie_node * {
            if (node == nullptr) {
                return nullptr;
            }

            if (node->is_terminal) {
                return node;
            }

            for (trie_node *child : node->children) {
                trie_node *result = dfs(child);
                if (result) {
                    return result;
                }
            }

            return nullptr;
        };
        return dfs(curr_node);
    };

    const trie_node* found_node_children = nullptr;
    for (auto child : current_node->children) {
        if (child) {
            found_node_children = findTerminalChild(child);
            if (found_node_children) {
                break;
            }
        }
    }

    if (found_node_children) {
        current_node = const_cast<trie_node*>(found_node_children);
        return *this;
    }

    // Lambda function to find a terminal child in the parent nodes
    auto findTerminalChildInParent = [&](trie_node* parent_node, char curr_value) -> trie_node* {
        std::function<trie_node*(trie_node*, char)> dfs_parent = [&](trie_node* node, char value) -> trie_node* {
            if (node == nullptr) {
                return nullptr;
            }

            for (size_t i = (int)value+1; i < num_chars; i++) {
                if (node->children[i]) {
                    trie_node* res = findTerminalChild(node->children[i]);
                    if (res) {
                        return res;
                    }
                }
            }

            return node->parent ? dfs_parent(node->parent, node->payload) : nullptr;
        };
        return dfs_parent(parent_node, curr_value);
    };

    trie_node* found_node_parent = findTerminalChildInParent(current_node->parent, current_node->payload);

    if (found_node_parent) {
        current_node = found_node_parent;
        return *this;
    } else {
        current_node = nullptr;
        return *this;
    }
}


trie::const_iterator trie::const_iterator::operator++(int) {
    trie::const_iterator temp = *this;
    ++(*this);
    return temp;
}

trie::const_iterator::reference trie::const_iterator::operator*() const {
    std::string res = "";
    const trie_node* curr_node = current_node;
    while(curr_node && curr_node->parent) {
        res = curr_node->payload + res;
        curr_node = curr_node->parent;
    }
    return res;
}

bool trie::const_iterator::operator==(const const_iterator& rhs) const {
    return current_node == rhs.current_node;
}

bool trie::const_iterator::operator!=(const const_iterator& rhs) const {
    return !(current_node == rhs.current_node);
}

void trie::swap(trie& rhs) {
    std::swap(m_root, rhs.m_root);
    std::swap(m_size, rhs.m_size);
}

bool trie::operator==(const trie& rhs) const {
    if(m_root && !rhs.m_root) {
        return false;
    }

    if(!m_root && rhs.m_root) {
        return false;
    }

    if(!m_root && !rhs.m_root) {
        return true;
    }

    if(m_size != rhs.m_size) {
        return false;
    }

    auto it = this->begin();
    auto et = this->end();

    auto it_rhs = rhs.begin();
    auto et_rhs = rhs.end();

    while(true) {
        if(*it != *it_rhs) {
            return false;
        }

        if(*it == *et && *it_rhs == *et_rhs) {
            return true;
        }

        ++it;
        ++it_rhs;
    }
}

bool trie::operator<(const trie& rhs) const {
    if(m_size > 0 && rhs.m_size == 0) {
        return false;
    }

    if(m_size == 0 && rhs.m_size > 0) {
        return true;
    }

    if(m_size == 0 && rhs.m_size == 0) {
        return false;
    }

    auto it = this->begin();
    auto et = this->end();

    auto it_rhs = rhs.begin();
    auto et_rhs = rhs.end();

    while(true) {
        if(it == et && it_rhs == et_rhs) {
            return false;
        }

        if(it == et && it_rhs != et_rhs) {
            return true;
        }

        if(it != et && it_rhs == et_rhs) {
            return false;
        }

        if(*it < *it_rhs) {
            return true;
        }

        if(*it_rhs < *it) {
            return false;
        }

        ++it;
        ++it_rhs;
    }
}

bool operator!=(const trie& lhs, const trie& rhs) {
    return !(lhs == rhs);
}

bool operator<=(const trie& lhs, const trie& rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

bool operator>(const trie& lhs, const trie& rhs) {
    return !(lhs <= rhs);
}

bool operator>=(const trie& lhs, const trie& rhs) {
    return (lhs > rhs) || (lhs == rhs);
}

std::ostream& operator<<(std::ostream& out, trie const& trie) {
    for (auto it = trie.begin(); it != trie.end(); ++it) {
        out << *it << " ";
    }
    return out;
}

// Stage 4
std::vector<std::string> trie::search_by_prefix(const std::string& prefix) const {
    std::vector<std::string> results;

    trie_node* curr_node = m_root;
    int len = prefix.size();
    int str_idx = 0;

    while(true) {
        if(prefix.empty()) {
            break;
        }
        int ascii_idx = (int)prefix[str_idx];
        trie_node* child = curr_node->children[ascii_idx];
        if(!child) {
            return results;
        }

        curr_node = child;
        str_idx++;

        if(str_idx == len) {
            break;
        }
    }

    std::function<void(trie_node*, std::string)> search_in_subtree = [&](trie_node* node, std::string current_word) {
        if(node->is_terminal) {
            results.push_back(current_word);
        }

        for(const auto child : node->children) {
            if(child) {
                search_in_subtree(child, current_word + child->payload);
            }
        }
    };

    search_in_subtree(curr_node, prefix);

    return results;
}


std::vector<std::string> trie::get_prefixes(const std::string& str) const {
    std::vector<std::string> results;

    if(str.empty()) {
        return results;
    }

    trie_node* curr_node = m_root;
    int len = str.size();
    int str_idx = 0;
    std::string word = "";

    if(m_root->is_terminal) {
        results.push_back(word);
    }

    while(true) {
        int ascii_index = (int)str[str_idx];
        trie_node* child = curr_node->children[ascii_index];

        if(child) {
            word = word + child->payload;
            if(child->is_terminal) {
                results.push_back(word);
            }
            curr_node = child;
        }

        str_idx++;
        if(str_idx >= len) {
            break;
        }
    }

    return results;
}

trie trie::operator&(trie const& rhs) const {
    trie result;
    if(m_size == 0 || rhs.m_size == 0) {
        return result;
    }

    trie_node* curr_node = m_root;
    trie_node* rhs_curr_node = rhs.m_root;
    trie_node* new_curr_node = result.m_root;

    std::function<void(trie_node*, trie_node*, trie_node*)> intersect_nodes = [&](trie_node* curr_node, trie_node* rhs_curr_node, trie_node* new_curr_node) {
        for(int i = 0; i < num_chars; i++) {
            trie_node* child = curr_node->children[i];
            trie_node* rhs_child = rhs_curr_node->children[i];

            if(!child || !rhs_child) {
                continue;
            }

            trie_node* new_child = new trie_node();
            new_child->payload = child->payload;
            new_child->parent = new_curr_node;
            new_curr_node->children[i] = new_child;
            if(child->is_terminal && rhs_child->is_terminal) {
                new_curr_node->children[i]->is_terminal = true;
                result.m_size++;
            }
            intersect_nodes(child, rhs_child, new_curr_node->children[i]);
        }
    };

    intersect_nodes(curr_node, rhs_curr_node, new_curr_node);

    return result;
}

trie trie::operator|(trie const& rhs) const {
    trie result;
    if(m_size == 0 && rhs.m_size == 0) {
        return result;
    }

    trie_node* curr_node = m_root;
    trie_node* rhs_curr_node = rhs.m_root;
    trie_node* new_curr_node = result.m_root;

    std::function<void(trie_node*, trie_node*)> union_nodes = [&](trie_node* curr_node, trie_node* new_curr_node) {
        for(int i = 0; i < num_chars; i++) {
            trie_node* child = curr_node->children[i];
            if(!child) {
                continue;
            }

            if(new_curr_node->children[i]) {
                if(child->is_terminal && !new_curr_node->children[i]->is_terminal) {
                    new_curr_node->children[i]->is_terminal = true;
                    result.m_size++;
                }
            }

            if(!new_curr_node->children[i]) {
                trie_node* new_child = new trie_node();
                new_child->payload = child->payload;
                new_child->parent = new_curr_node;
                new_curr_node->children[i] = new_child;
                if(child->is_terminal) {
                    new_curr_node->children[i]->is_terminal = true;
                    result.m_size++;
                }
            }

            union_nodes(child, new_curr_node->children[i]);
        }
    };

    if(m_size != 0) {
        union_nodes(curr_node, new_curr_node);
    }
    if(rhs.m_size != 0) {
        union_nodes(rhs_curr_node, new_curr_node);
    }

    return result;
}