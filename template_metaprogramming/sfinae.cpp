#include <iostream>
#include <type_traits>

// Overload for integer types
template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
checkType(T t) {
    std::cout << "Integral type!" << std::endl;
}

// Overload for non-integer types
template <typename T>
typename std::enable_if<!std::is_integral<T>::value, void>::type
checkType(T t) {
    std::cout << "Non-integral type!" << std::endl;
}

int main() {
    checkType(42);        // Calls the integral version
    checkType(3.14);      // Calls the non-integral version
    return 0;
}