#include <iostream>

using namespace std;

class CFirma {
public:
	CFirma (void);
	bool vloz (CZamestnanec & a);
	bool odeber (const CZamestnanec & a);
	void vypis (void);
	void platy (void);
private:
	CZamestnanec ** m_List;
	int m_Count;
	int m_MaxCount;
	int m_Size;
};

CFirma::CFirma (void) {
	m_Size = 100;
	m_Count = 0;
	m_MaxCount = 0;
	m_List = new CZamestnanec * [m_Size];
}

bool CFirma::vloz (CZamestnanec & a) {
	if (m_Count == m_Size) {
		CZamestnanec ** tmp = new CZamestnanec * [m_Size * 2];
		for (int i = 0; i < m_Count; i++)
			tmp[i] = m_List[i];

		delete [] m_List;

		m_List = tmp;
	}

	m_List[m_Count] = & a;
	m_Count++;
	
	if (m_Count > m_MaxCount)
		m_MaxCount = m_Count;

	return true;
}

bool CFirma::odeber (const CZamestnanec & a) {
	for (int i = 0; i < m_Count; i++) {
		if (m_List[i]->Compare(a)) {
			for (int j = i; j < m_Count; j++)
				m_List[j] = m_List[j + 1];

			m_Count--;

			return true;
		}
	}

	return false;
}

void CFirma::vypis (void) {
	cout << "-------------------------------------" << endl;
	for (int i = 0; i < m_Count; i++)
		m_List[i]->vypis();
}

void CFirma::platy (void) {
	long int res = 0;
	for (int i = 0; i < m_Count; i++)
		res += m_List[i]->plat();

	cout << "Soucet platu: " << res << endl;
	cout << "-------------------------------------" << endl;
}
