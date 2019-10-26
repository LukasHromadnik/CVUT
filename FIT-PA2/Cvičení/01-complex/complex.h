//
//  complex.h
//  test
//
//  Created by Lukáš Hromadník on 27.02.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#ifndef __COMPLEX_H__
#define __COMPLEX_H__

class Complex {
private:
    int x;
    int i;
    
public:
    Complex();
    Complex(int x, int i = 0);
    
    int getRe() const;
    int getIm() const;
    
    float abs() const;
    void print() const;
    
    Complex add(Complex a);
    Complex diff(Complex b);
//    friend Complex operator + (Complex a, Complex b);
//    friend Complex operator - (Complex a, Complex b);
    Complex operator + (Complex a);
    Complex operator - (Complex a);
    friend ostream & operator << (ostream & os, Complex a);
    
    Complex operator++ (); //++a
    Complex operator++ (int); //a++
};

Complex add(Complex a, Complex b);
Complex diff(Complex a, Complex b);

//Complex operator + (Complex a, Complex b);
//Complex operator - (Complex a, Complex b);
ostream & operator << (ostream & os, Complex a);

#endif
