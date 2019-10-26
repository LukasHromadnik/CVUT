//
//  main.cpp
//  cviko2
//
//  Created by Lukáš Hromadník on 06.03.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#include <iostream>

using namespace std;

#include "frontaPolem.h"


int main() {
    Fronta kladna, zaporna;
    int cislo;
    cout << "zadejte radu celych cisel zakoncenou nulou" << endl;
    cin >> cislo;
    while (cislo) {
        if (cislo>0) kladna.vloz(cislo);
        else zaporna.vloz(cislo);
        cin >> cislo;
    }
    cout << "kladna cisla:" << endl;
    while (!kladna.jePrazdna()) {
        cislo = kladna.odeber();
        cout << cislo << ' ';
    }
    cout << "\nzaporna cisla:" << endl;
    while (!zaporna.jePrazdna()) {
        cislo = zaporna.odeber();
        cout << cislo << ' ';
    }
    cout << endl;
    
    return 0;
}
