#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Parkoviste {
public:
	Parkoviste (int osobnich, int nakladnich);

	void zaparkuj (const Nakladni &);
	void zaparkuj (const Osobni &);

	Vehicle * najdi (string spz);

	void odjed (string spz);

	void vypis();

private:
	vector<Vehicle *> m_List;
	int m_Osobnich;
	int m_Nakladnich;
	int m_CurrOsobnich;
	int m_CurrNakladnich;
	int m_ResNakladnich;
};

Parkoviste::Parkoviste (int osobnich, int nakladnich) {
	m_Osobnich = osobnich;
	m_Nakladnich = nakladnich;
	m_CurrNakladnich = 0;
	m_CurrOsobnich = 0;
	m_ResNakladnich = 0;
}

void Parkoviste::zaparkuj (const Nakladni & a) {
	if (m_CurrNakladnich == (m_Nakladnich - m_ResNakladnich))
		throw "Neni misto k zaparkovni nakladniho automobilu.";

	m_List.push_back(& a);
	m_CurrNakladnich++;
}

void Parkoviste::zaparkuj (const Osobni & a) {
	if (m_CurrOsobnich == m_Osobnich && m_CurrNakladnich == m_Nakladnich)
		throw "Neni misto k zaparkovani osobniho automobilu.";

	if (m_CurrOsobnich == m_Osobnich)
		m_ResNakladnich--;

	m_List.push_back(& a);
	m_CurrOsobnich++;

}

Vehicle * Parkoviste::najdi (string spz) {
	for (int i = 0; i < m_List.size(); i++)
		if (m_List.at(i) == spz)
			return (* m_List.at(i));

	throw "Auto nenalezeno.";
}

void Parkoviste::odjed (string spz) {
	for (int i = 0; i < m_List.size(); i++)
		if (m_List.at(i) == spz)
			m_List.remove(i);

	throw "Auto nenalzeno.";
}

void Parkoviste::vypis (void) {
	for (int i = 0; i < m_List.size(); i++)
		cout << (* m_List.at(i));
}