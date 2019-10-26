#include <iostream>
#include <cmath>

using namespace std;

#include "complex.h"

Complex::Complex(void) {
    this->x = 0;
    this->i = 0;
}

Complex::Complex(int x, int i) {
    this->x = x;
    this->i = i;
}

float Complex::abs() const {
    return sqrt(pow(this->x, 2) + pow(this->i, 2));
}

void Complex::print(void) const {
    cout << this->x;
    
    if (this->i > 0)
        cout << " + " << this->i;
    else
        cout << this->i;
    
    cout << "i";
    
}

int Complex::getRe() const {
    return this->x;
}

int Complex::getIm() const {
    return this->i;
}

Complex add(Complex a, Complex b) {
    Complex tmp(a.getRe() + b.getRe(), a.getIm() + b.getIm());
    
    return tmp;
}

Complex diff(Complex a, Complex b) {
    Complex tmp(a.getRe() - b.getRe(), a.getIm() - b.getIm());
    
    return tmp;
}

//Complex operator + (Complex a, Complex b) {
//    Complex tmp(a.x + b.x, a.i + b.i);
//    
//    return tmp;
//}
//
//Complex operator - (Complex a, Complex b) {
//    Complex tmp(a.x - b.x, a.i - b.i);
//    
//    return tmp;
//}

Complex Complex::operator + (Complex a) {
    return Complex(x + a.x, i + a.i);
}

Complex Complex::operator - (Complex a) {
    return Complex(x - a.x, i - a.i);
}

Complex Complex::operator++ () {
    return Complex(++x, i);
}

Complex Complex::operator++ (int) {
    Complex tmp(x, i);
    x++;
    return tmp;
}

ostream & operator << (ostream & os, Complex a) {
    os << a.x;
    
    if (a.i > 0)
        os << " + " << a.i;
    else
        os << a.i;
    
    os << "i";
    
    return os;
}

int main(void) {
    Complex x(1, 2);
    
    cout << x << endl;
    cout << ++x << endl;
    cout << x++ << endl;
    cout << x << endl;
    return 0;
}