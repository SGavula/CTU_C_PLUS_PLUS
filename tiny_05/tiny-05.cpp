#include "tiny-05.hpp"

namespace pjc {

    complex::complex(double real, double imaginary) :
            m_real(real),
            m_imag(imaginary) {}

    double complex::real() const {
        return m_real;
    }

    void complex::real(double d) {
        m_real = d;
    }

    double complex::imag() const {
        return m_imag;
    }

    void complex::imag(double d) {
        m_imag = d;
    }

    // Addition
    complex operator + (double d_num, complex c_num) {
        return complex(d_num + c_num.real(), c_num.imag());
    }

    // Subtraction
    complex operator - (double d_num, complex c_num) {
        return complex(d_num - c_num.real(), -c_num.imag());
    }

    // Multiplication
    complex operator * (double d_num, complex c_num) {
        return complex(d_num * c_num.real(), d_num * c_num.imag());
    }
}

