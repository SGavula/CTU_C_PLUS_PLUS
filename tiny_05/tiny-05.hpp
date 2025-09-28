#pragma once

namespace pjc {

/**
 * Jednoduchá implementace komplexních čísel, viz třeba
 * https://cs.wikipedia.org/wiki/Komplexn%C3%AD_%C4%8D%C3%ADslo
 *
 * Vaším úkolem je pro ni přidat binární operátory +, -, *, fungující
 * pro pár instancí complex, ale i pro kombinaci instance complex a
 * double.
 */
class complex {
public:

    complex(double real, double imaginary);

    // Addition
    complex operator + (complex c_num) {
        double c_m_real = c_num.real();
        double c_m_imag = c_num.imag();

        return complex(m_real + c_m_real, m_imag + c_m_imag);
    }

    complex operator + (double d_num) {
        return complex(m_real + d_num, m_imag);
    }

    // Subtraction
    complex operator - (complex c_num) {
        double c_m_real = c_num.real();
        double c_m_imag = c_num.imag();

        return complex(m_real - c_m_real, m_imag - c_m_imag);
    }

    complex operator - (double d_num) {
        return complex(m_real - d_num, m_imag);
    }

    // Multiplication
    complex operator * (complex c_num) {
        double c_m_real = c_num.real();
        double c_m_imag = c_num.imag();

        double new_m_real = (m_real * c_m_real) - (m_imag * c_m_imag);
        double new_m_imag = (m_real * c_m_imag) + (m_imag * c_m_real);

        return complex(new_m_real, new_m_imag);
    }

    complex operator * (double d_num) {
        return complex(m_real * d_num, m_imag * d_num);
    }

    // Vrátí reálnou část
    double real() const;
    // Nastaví reálnou část na d
    void real(double d);

    // Vrátí imaginární část
    double imag() const;
    // Nastaví imaginární část na d
    void imag(double d);



private:
    double m_real;
    double m_imag;
};

    complex operator + (double d_num, complex c_num);
    complex operator - (double d_num, complex c_num);
    complex operator * (double d_num, complex c_num);
} // end namespace pjc
