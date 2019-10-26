//
//  main.cpp
//  cviko5
//
//  Created by Lukáš Hromadník on 20.03.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#include <iostream>
#include "Bag.h"

using namespace std;

int main() {
    Bag b1;
    b1.vloz(1);
    b1.vloz(3);
    b1.vloz(1);
//    b1.vloz(2);
    b1 += 2;
    cout << "b1 = " << b1 << endl;
//    Bag b2 = b1;
    Bag b2(b1);
//    cout << "b2 = " << b2 << endl;
    b1.odeber(1);
    cout << "b1 = " << b1 << endl;
    cout << "b2 = " << b2 << endl;
    Bag b3;
    b3 = b1;
    b1.odeber(1);
    cout << "b1 = " << b1 << endl;
    cout << "b3 = " << b3 << endl;
    
    int a = 1;
    cout << "b1 contains " << a << "? " << ((b1.jeTam(a)) ? "Yes" : "No") << endl;
//    system("PAUSE");
    return 0;
}

