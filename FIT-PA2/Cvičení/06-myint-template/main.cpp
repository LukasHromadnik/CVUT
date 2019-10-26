#include <iostream>
#include "myint.hpp"
using namespace std;

// template<class T>
// int porovnej(const T & a, const T & b) {
// 	return a < b;
// } 

template<class T>
T * findElem (T * p, int n, const T & x) {
	for (int i = 0; i < n; i++) {
		if (p[i] == x)
			return p + i;
	}

	return NULL;
}

// template<class T>
// class Trida {
// public:
// 	Trida(void);
// };

// template<class T>
// Trida<T>::Trida() {

// }

void f () {
	if ()
		throw Exc(); 
}

int main() {
	// int n = 5;
	// MyInt * a;
	// MyInt array[5];

	// for (int i = 0; i < n; i++)
	// 	array[i] = MyInt (i);

	// a = array;

	// MyInt b = findElem(a, n, MyInt(3));

	// cout << b << endl;

	try {
		f();
	}
	catch (const Exc & x) {

	}


	return 0;
}
