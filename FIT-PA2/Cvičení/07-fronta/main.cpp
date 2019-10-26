#include <iostream>
// #include <stdlid.h>
#include "queue.h"
using namespace std;
typedef struct Spojak {
	Spojak (int val, Spojak * next);
	int m_Val;
	Spojak * m_Next;
} Spojak;

Spojak::Spojak(int val, Spojak * next = NULL) {
	m_Val = val;
	m_Next = next;
}

int main() {
	Queue<Spojak> kladna, zaporna;
	int cislo;
	
	cout << "zadejte radu celych cisel zakoncenou nulou" << endl;
	cin >> cislo;

	while (cislo) {
		if (cislo > 0) kladna.insert(cislo);
		else zaporna.insert(cislo);
		cin >> cislo;
	}
	cout << "kladna cisla:" << endl;

	while ( ! kladna.isEmpty()) {
		cislo = kladna.remove();
		cout << cislo << ' ';
	}
	cout << "\nzaporna cisla:" << endl;

	while ( ! zaporna.isEmpty()) {
		cislo = zaporna.remove();
		cout << cislo << ' ';
	}
	cout << endl;

	return 0; 
}
