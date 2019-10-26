#include <iostream>
using namespace std;

class MyInt {
public:
	MyInt (int val = 0);
	bool operator == (const MyInt & a);
	friend ostream & operator << (ostream & os, const MyInt & a);

private:
	int * p_val;

};

MyInt::MyInt (int val) {
	p_val = new int (val);
}

bool MyInt::operator == (const MyInt & a) {
	return (* p_val == * a.p_val);
}

ostream & operator << (ostream & os, const MyInt & a) {
	os << * a.p_val;

	return os;
}
