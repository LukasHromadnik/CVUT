//
//  fronta.h
//  cviko2
//
//  Created by Lukáš Hromadník on 06.03.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#ifndef cviko2_fronta_h
#define cviko2_fronta_h

class Fronta {
public:
    void vloz(int cislo);
    int jePrazdna() const;
    int odeber();
    Fronta();
    ~Fronta();
    
private:
    struct Elem {
        int val;
        Elem * dalsi;
        Elem(int _val, Elem * _dalsi = NULL);
    };
    
    Elem * head;
    Elem * tail;
};

Fronta::Elem::Elem(int _val, Elem * _dalsi) {
    val = _val;
    dalsi = _dalsi;
}

Fronta::Fronta() {
    this->head = NULL;
    this->tail = NULL;
}

Fronta::~Fronta() {
    Elem * tmp = NULL;
    
    while (this->head) {
        tmp = this->head->dalsi;
        
        delete this->head;
        
        this->head = tmp;
    }
}

void Fronta::vloz(int cislo) {
    Fronta::Elem * element = new Fronta::Elem(cislo);
    
    if (this->jePrazdna())
        this->head = element;
    else
        this->tail->dalsi = element;
    
    this->tail = element;
}

int Fronta::jePrazdna() const {
    return ! this->head;
}

int Fronta::odeber() {
    int result;
    Elem * element = this->head;
    
    this->head = element->dalsi;
    result = element->val;
    
    delete element;
    
    return result;
}



#endif
