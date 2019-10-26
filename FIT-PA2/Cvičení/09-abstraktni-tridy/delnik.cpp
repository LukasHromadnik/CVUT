#include <iostream>
// #include "zamestnanec.cpp"

using namespace std;

class CDelnik : public CZamestnanec {
private:
	int m_Mzda;
	int m_PocetHodin;
public:
	CDelnik (const char * jmeno, const char * prijmeni, int id, int mzda, int pocetHodin);
	virtual int plat (void);
	virtual void vypis (void);
};

CDelnik::CDelnik (const char * jmeno, const char * prijmeni, int id, int mzda, int pocetHodin) : CZamestnanec (jmeno, prijmeni, id) {
	m_Mzda = mzda;
	m_PocetHodin = pocetHodin;
}

int CDelnik::plat (void) {
	return m_Mzda * m_PocetHodin;
}

void CDelnik::vypis (void) {
	cout << "Pozice: Delnik" << endl;

	CZamestnanec::vypis();

	cout << "Mzda: " << m_Mzda << endl;
	cout << "Pocet hodin: " << m_PocetHodin << endl;
	cout << "Plat: " << plat() << endl;
	cout << "-------------------------------------" << endl;
}
