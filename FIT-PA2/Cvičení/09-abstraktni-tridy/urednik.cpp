#include <iostream>
// #include "zamestnanec.cpp"

using namespace std;

class CUrednik : public CZamestnanec {
private:
	int m_Mzda;
public:
	CUrednik (const char * jmeno, const char * prijmeni, int id, int mzda);
	virtual int plat (void);
	virtual void vypis (void);
};

CUrednik::CUrednik (const char * jmeno, const char * prijmeni, int id, int mzda) : CZamestnanec (jmeno, prijmeni, id) {
	m_Mzda = mzda;
}

int CUrednik::plat (void) {
	return m_Mzda;
}

void CUrednik::vypis (void) {
	cout << "Pozice: Urednik" << endl;

	CZamestnanec::vypis();

	cout << "Mzda: " << m_Mzda << endl;
	cout << "Plat: " << plat() << endl;
	cout << "-------------------------------------" << endl;
}
