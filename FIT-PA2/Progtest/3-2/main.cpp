#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

#endif /* __PROGTEST__ */

class CCoord {
public:
	CCoord ( double lat, double lon );
};

class CGPS {
public:
	friend ostream & operator << (ostream & os, const CGPS & a);
};

ostream & operator << (ostream & os, const CGPS & a) {
	os << '[';
	os << ']';

	return os;
}

int main (void) {
	CGPS x0;

	cout << x0 << endl; 

	return 0;
}
