#include <iostream>
#include <string>

using namespace std;

class Vehicle {
protected:
	string m_SPZ;

public:
	Vehicle (string m_SPZ);
	friend ostream & operator << (ostream & os, const Vehicle & a);
	void operator == (const Vehicle & a);
};

Vehicle::Vehicle (string spz) {
	m_SPZ = spz;
}

ostream & operator << (ostream & os, const Vehicle & a) {
	os << a.m_SPZ << endl;

	return os;
}

void Vehicle::operator == (const Vehicle & a) {
	return m_SPZ.compare(a.m_SPZ);
}
