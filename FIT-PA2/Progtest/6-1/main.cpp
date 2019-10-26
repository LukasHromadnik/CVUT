#ifndef __PROGTEST__
#include "common.h"
using namespace std;
#endif /* __PROGTEST__ */

// ------------------------------------------------------------------------------------------

class CRec {
public:
	string Name (void) const;
	virtual string Type (void) const = 0;
	friend ostream & operator << (ostream &, const CRec &);
	virtual void Print (ostream &) const;
	virtual CRec * Clone (void) const = 0;
	bool EqualName (string name) const;
	bool operator == (const CRec &) const;
	virtual bool Equal (const CRec &) const = 0;
protected:
	string m_Name;
};

string CRec::Name (void) const {
	// 
	return m_Name;
}

void CRec::Print (ostream & os) const {
	// 
	os << Name() << endl;
}

bool CRec::EqualName (string name) const {
	// 
	return (m_Name == name);
}

ostream & operator << (ostream & os, const CRec & a) {
	a.Print(os);

	return os;
}

bool CRec::operator == (const CRec & a) const {
	// cout << "tu" << endl;
	return a.Equal( * this);
	// return ( * this).Equal(a);
}

// ------------------------------------------------------------------------------------------

class VRec {
public:
	VRec (void);
	friend ostream & operator << (ostream &, const VRec &);
	int Count (void);
	bool Add (const CRec *);
	const CRec & operator [] (int x);
private:
	vector<const CRec * > m_List;
	int m_Count;
};

VRec::VRec (void) {
	// 
	m_Count = 0;
}

int VRec::Count (void) {
	// 
	return m_Count;
}

bool VRec::Add (const CRec * a) {
	m_List.push_back(a);
	m_Count++;

	return true;
}

ostream & operator << (ostream & os, const VRec & a) {
	for (auto it : a.m_List)
		os << (* it);

	return os;
}

const CRec & VRec::operator [] (int x) {
	if (x < m_Count)
		return ( * m_List.at(x));
	else
		throw InvalidIndexException (x);
}

// ------------------------------------------------------------------------------------------

class CRecA : public CRec {
public:
	CRecA (string name, const CIPv4 &);
	CRecA (const CRecA &);
	virtual string Type (void) const;
	virtual void Print (ostream &) const;
	virtual CRec * Clone (void) const;
	virtual bool Equal (const CRec &) const;
private:
	CIPv4 m_IP;
};

CRecA::CRecA (string name, const CIPv4 & ip) : m_IP (ip) {
	// 
	m_Name = name;
}

CRecA::CRecA (const CRecA & a) : m_IP (a.m_IP) {
	// 
	m_Name = a.m_Name;
}

string CRecA::Type (void) const {
	// 
	return "A";
}

void CRecA::Print (ostream & os) const {
	// 
	os << Name() << " " << Type() << " " << m_IP << endl;
}

CRec * CRecA::Clone (void) const {
	// 
	return new CRecA (*this);
}

bool CRecA::Equal (const CRec & a) const {
	if (Type() != a.Type())
		return false;

	const CRecA * tmp = dynamic_cast<const CRecA * >( & a);

	return (m_Name == tmp->m_Name && m_IP == tmp->m_IP);
}

// ------------------------------------------------------------------------------------------

class CRecAAAA : public CRec {
public:
	CRecAAAA (string name, const CIPv6 &);
	CRecAAAA (const CRecAAAA &);
	virtual string Type (void) const;
	virtual void Print (ostream &) const;
	virtual CRec * Clone (void) const;
	virtual bool Equal (const CRec &) const;
private:
	CIPv6 m_IP;
};

CRecAAAA::CRecAAAA (string name, const CIPv6 & ip) : m_IP (ip) {
	// 
	m_Name = name;
}

CRecAAAA::CRecAAAA (const CRecAAAA & a) : m_IP (a.m_IP) {
	// 
	m_Name = a.m_Name;
}

string CRecAAAA::Type (void) const {
	// 
	return "AAAA";
}

void CRecAAAA::Print (ostream & os) const {
	// 
	os << Name() << " " << Type() << " " << m_IP << endl;
}

CRec * CRecAAAA::Clone (void) const {
	// 
	return new CRecAAAA (*this);
}

bool CRecAAAA::Equal (const CRec & a) const {
	if (Type() != a.Type())
		return false;

	const CRecAAAA * tmp = dynamic_cast<const CRecAAAA * >( & a);

	return (m_Name == tmp->m_Name && m_IP == tmp->m_IP);
}

// ------------------------------------------------------------------------------------------

class CRecMX : public CRec {
public:
	CRecMX (string name, string exchanger, int preference);
	CRecMX (const CRecMX &);
	virtual string Type (void) const;
	virtual void Print (ostream &) const;
	virtual CRec * Clone (void) const;
	virtual bool Equal (const CRec &) const;
private:
	string m_Exchanger;
	int m_Preference;
};

CRecMX::CRecMX (string name, string exchanger, int preference) {
	m_Name = name;
	m_Exchanger = exchanger;
	m_Preference = preference;
}

CRecMX::CRecMX (const CRecMX & a) {
	m_Name = a.m_Name;
	m_Exchanger = a.m_Exchanger;
	m_Preference = a.m_Preference;
}

string CRecMX::Type (void) const {
	// 
	return "MX";
}

void CRecMX::Print (ostream & os) const {
	// 
	os << Name() << " " << Type() << " " << m_Preference << " " << m_Exchanger << endl;
}

CRec * CRecMX::Clone (void) const {
	// 
	return new CRecMX (*this);
}

bool CRecMX::Equal (const CRec & a) const {
	if (Type() != a.Type())
		return false;

	const CRecMX * tmp = dynamic_cast<const CRecMX * >( & a);

	return (m_Name == tmp->m_Name && m_Preference == tmp->m_Preference && m_Exchanger == tmp->m_Exchanger);
}

// ------------------------------------------------------------------------------------------

class CZone {
public:
	CZone (string name);
	// ~CZone (void);
	bool Add (const CRec &);
	bool Del (const CRec &);
	int Count (void) const;
	VRec Search (string name) const;
	friend ostream & operator << (ostream &, const CZone &);
private:
	string m_Name;
	vector<const CRec * > m_List;
	int m_Count;
};

CZone::CZone (string name) {
	m_Name = name;
	m_Count = 0;
}

// CZone::~CZone (void) {
// 	for (auto it : m_List)
// 		delete (* it);
// }

bool CZone::Add (const CRec & a) {
	for (auto it = m_List.begin(); it != m_List.end(); it++)
		if (( ** it) == a)
			return false;

	m_List.push_back(a.Clone());
	m_Count++;

	return true;
}

bool CZone::Del (const CRec & a) {
	for (auto it = m_List.begin(); it != m_List.end(); it++) {
		if (( ** it) == a) {
			m_List.erase(it);
			m_Count--;
			return true;
		}
	}

	return false;
}

int CZone::Count (void) const {
	// 
	return m_Count;
}

VRec CZone::Search (string name) const {
	VRec * tmp = new VRec();
	for (auto it : m_List)
		if (it->EqualName(name))
			tmp->Add(it);

	return (* tmp);
}

ostream & operator << (ostream & os, const CZone & a) {
	int counter = 1;
	os << a.m_Name << endl;

	for (auto it : a.m_List)
		os << " " << ((counter++ == a.Count()) ? "\\-" : "+-") << " " << (* it);

	return os;
}

// ------------------------------------------------------------------------------------------

#ifndef __PROGTEST__
int main ( void ) {
	#include "tests.inc"

 	return 0;
}
#endif /* __PROGTEST__ */
