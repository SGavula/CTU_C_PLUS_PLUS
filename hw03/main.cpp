#include <iostream>
#include "trie.hpp"

int main() {
    // Here you can write your main program logic
    trie trie;
    trie.insert("a");
  //  std::cout << trie << std::endl;

    trie.erase("a");

    std::cout << trie << std::endl;

    return 0;
}