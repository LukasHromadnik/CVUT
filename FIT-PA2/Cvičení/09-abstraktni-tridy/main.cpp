#include <iostream>
#include "zamestnanec.cpp"
#include "delnik.cpp"
#include "urednik.cpp"
#include "firma.cpp"

using namespace std;

int main () {
	CUrednik a("Tomáš", "Sokol", 123, 12000);
	CDelnik b("Lukáš", "Hromadník", 321, 90, 100);
	CFirma c;

	// a.vypis();
	// b.vypis();

	c.vloz(a);
	c.vloz(b);
	c.vypis();
	c.platy();

	c.odeber(a);
	c.vypis();
	c.platy();

	return 0;
}
