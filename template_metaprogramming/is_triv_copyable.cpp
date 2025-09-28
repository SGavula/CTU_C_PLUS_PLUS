#include <iostream>
#include <type_traits>  // std::is_trivially_copyable

// Your template definition
template <typename T>
struct is_trivially_copyable : std::is_trivially_copyable<T> {};

// Trivially copyable (no custom constructor)
struct A { 
    int x;
};
// Not trivially copyable (custom constructor)
struct B { 
    B() {}
    B(const B&) {}  
};

// Usage in code
int main() {
    std::cout << std::boolalpha;  // Print bools as "true"/"false"
    
    // Check if types are trivially copyable
    std::cout << "Is A trivially copyable? " << is_trivially_copyable<A>::value << std::endl;
    std::cout << "Is B trivially copyable? " << is_trivially_copyable<B>::value << std::endl;
    std::cout << "Is int trivially copyable? " << is_trivially_copyable<int>::value << std::endl;  // true
    
   