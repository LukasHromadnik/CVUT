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

complex operator+(const complex& a, const complex& b) {
    return complex(a.real() + b.real(), a.imag() + b.imag());
}

complex operator-(const complex& a, const complex& b) {
    return complex(a.real() - b.real(), a.imag() - b.imag());
}

complex operator*(const complex& a, const complex& b) {
    double real = a.real() * b.real() - a.imag() * b.imag();
    double imag = a.real() * b.imag() + b.real() * a.imag();
    return complex(real, imag);
}

complex operator+(const complex& a, const double& b) {
    complex rhs = complex(b, 0);
    return a + rhs;
}

complex operator-(const complex& a, const double& b) {
    complex rhs = complex(b, 0);
    return a - rhs;
}

complex operator*(const complex& a, const double& b) {
    complex rhs = complex(b, 0);
    return a * rhs;
}

complex operator+(const double& a, const complex& b) {
    complex lhs = complex(a, 0);
    return lhs + b;
}

complex operator-(const double& a, const complex& b) {
    complex lhs = complex(a, 0);
    return lhs - b;
}

complex operator*(const double& a, const complex& b) {
    complex lhs = complex(a, 0);
    return lhs * b;
}

}
