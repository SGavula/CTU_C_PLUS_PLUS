// Online C++ compiler to run C++ program online
#include <iostream>

template<int N>
struct Factorial {
    static const int test = N * Factorial<N - 1>::test;
};


template<>
struct Factorial<1> {
    static const int test = 1; 
};

template<int A, int B>
struct Add {
    static const int value = A + B;
};

template<int K>
struct TwoToSomething {
    static const int result = 2 * TwoToSomething<K - 1>::result;
};

template<>
struct TwoToSomething<0> {
    static const int result = 1;
};

int main() {
    // Write C++ code here
    std::cout << "Try programiz.pro" << std::endl;
    std::cout << Factorial<5>::test << std::endl;
    std::cout << Add<5, 4>::value << std::endl;
    std::cout << TwoToSomething<8>::result << std::endl;
    
    return 0;
}