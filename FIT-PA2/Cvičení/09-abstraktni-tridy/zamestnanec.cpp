#include <iostream>

using namespace std;

class CZamestnanec {
private:
	char * m_Jmeno;
	char * m_Prijmeni;
	int m_ID;
public:
	CZamestnanec (const char * jmeno, const char * prijmeni, int id);
	virtual int plat (void) = 0;
	virtual void vypis (void);
	bool Compare (const CZamestnanec & a);
};

CZamestnanec::CZamestnanec (const char * jmeno, const char * prijmeni, int id) {
	int charLen = 25;
	m_Jmeno = new char[charLen];
	strncpy(m_Jmeno, jmeno, charLen);
	
	charLen = 36;
	m_Prijmeni = new char[charLen];
	strncpy(m_Prijmeni, prijmeni, charLen);
	
	m_ID = id;
}

void CZamestnanec::vypis (void) {
	cout << "ID: " << m_ID << endl;
	cout << "Jmeno: " << m_Jmeno << endl;
	cout << "Prijmeni: " << m_Prijmeni << endl;
}

bool CZamestnanec::Compare (const CZamestnanec & a) {
	return (m_ID == a.m_ID);
}
