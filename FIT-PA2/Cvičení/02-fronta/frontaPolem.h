//
//  frontaPolem.h
//  cviko2
//
//  Created by Lukáš Hromadník on 06.03.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#ifndef cviko2_frontaPolem_h
#define cviko2_frontaPolem_h

class Fronta {
public:
    void vloz(int cislo);
    int jePrazdna() const;
    int odeber();
    Fronta();
    ~Fronta();

private:
    int count;
    int * arr;
    int head;
    int tail;
};

Fronta::Fronta() {
    this->count = 10;
    this->head = 0;
    this->tail = 0;
    this->arr = new int[this->count];
}

Fronta::~Fronta() {
    delete this->arr;
}

int Fronta::jePrazdna() const {
    return this->head == this->tail;
}

void Fronta::vloz(int cislo) {
    if (this->tail > this->count) {
        int * tmp = new int[2 * this->count];
        
        for (int i = 0; i < this->count; i++)
            tmp[i] = this->arr[i];
        
        this->count *= 2;
        delete this->arr;
        this->arr = tmp;
    }
    
    this->arr[this->tail] = cislo;
    this->tail++;
}

int Fronta::odeber() {
    int result;
    if (this->arr[this->head])
        result = this->arr[this->head];
    else
        result = NULL;
    
    this->head++;
    
    return result;
}



#endif
