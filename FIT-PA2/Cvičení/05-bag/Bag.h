//
//  Bag.h
//  cviko5
//
//  Created by Lukáš Hromadník on 20.03.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#ifndef cviko5_Bag_h
#define cviko5_Bag_h
using namespace std;

class Bag {
public:
    Bag();
    Bag(const Bag & orig);
    Bag & operator = (const Bag & orig);
    void operator += (int a);
    void vloz (int a);
    void odeber (int a);
    bool jeTam(int a);
    int pocet;
    int * data;
private:
    int size;
};

Bag::Bag() {
    pocet = 0;
    size = 10;
    data = new int [size];
}

Bag::Bag(const Bag & orig) {
    pocet = orig.pocet;
    size = orig.size;
    data = new int [pocet];
    
    for (int i = 0; i < pocet; i++)
        data[i] = orig.data[i];
}

void Bag::vloz(int a) {
    if (this->pocet + 1 > this->size) {
        this->size *= 2;
        int * newArr = new int [this->size];
        
        memcpy(newArr, this->data, (this->size / 2) * sizeof(this->data));
        
        delete [] this->data;
        this->data = newArr;
    }
    
    this->data[this->pocet] = a;
    this->pocet++;
}

void Bag::odeber (int a) {
    int odebrano = 0;
    for (int i = 0; i < this->pocet; i++) {
        if ( ! odebrano && this->data[i] == a) {
            this->pocet--;
            odebrano = 1;
        }
        
        if (odebrano)
            this->data[i] = this->data[i + 1];
    }
}

bool Bag::jeTam(int a) {
    for (int i = 0; i < this->pocet; i++) {
        if (this->data[i] == a)
            return true;
    }
    
    return false;
}

Bag & Bag::operator = (const Bag & orig) {
    if (& orig == this)
        return * this;
    
    delete [] data;
    
    pocet = orig.pocet;
    size = orig.size;
    
    
    for (int i = 0; i < pocet; i++)
        data[i] = orig.data[i];
    
    return * this;
}

void Bag::operator += (int a) {
    this->vloz(a);
}

ostream & operator << (ostream & os, Bag a) {
    for (int i = 0; i < a.pocet; i++) {
        os << a.data[i];
        
        if (i + 1 != a.pocet)
            os << ", ";
    }
    
    return os;
}

#endif
