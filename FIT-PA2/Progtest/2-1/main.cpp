#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

class CCitizen {
public:
	CCitizen (const string & name, const string & addr);
	CCitizen (const string & name, const string & addr, const string & account);
	void UpdateIncome (int amount);
	void UpdateExpense (int amount);
	void Print (void) const;
	bool operator < (CCitizen a);
	bool operator == (const CCitizen a);
	string m_Name;
	string m_Addr;
	string m_Account;
	int m_Income;
	int m_Expense;
};

CCitizen::CCitizen (const string & name, const string & addr) {
	m_Name = name;
	m_Addr = addr;
	m_Account = "";
	m_Income = 0;
	m_Expense = 0;
}

CCitizen::CCitizen (const string & name, const string & addr, const string & account) {
	m_Name = name;
	m_Addr = addr;
	m_Account = account;
	m_Income = 0;
	m_Expense = 0;
}

void CCitizen::Print (void) const {
	cout << m_Name << ", " << m_Addr << ", " << m_Account << ", " << m_Income << ", " << m_Expense << endl;
}

bool CCitizen::operator < (CCitizen a) {
	return (m_Name.compare(a.m_Name) > 0) ? true: false;
}

bool CCitizen::operator == (CCitizen a) {
	return (m_Name == a.m_Name && m_Addr == a.m_Addr);
}

void CCitizen::UpdateIncome (int amount) {
	m_Income += amount;
}

void CCitizen::UpdateExpense (int amount) {
	m_Expense += amount;
}

// ---------------------------------------------------------------------------------------------- //
class CTaxRegister {
public:
	CTaxRegister (void);
	~CTaxRegister (void);
	bool Birth (const string & name, const string & addr, const string & account);
	bool Death (const string & name, const string & addr);
	bool Income (const string & account, int amount);
	bool Income (const string & name, const string & addr, int amount);
	bool Expense (const string & account, int amount);
	bool Expense (const string & name, const string & addr, int amount);
	bool Audit (const string & name, const string & addr, string & account, int & sumIncome, int & sumExpense) const;
	void Print (void);
	void Sort(void);
	vector<CCitizen> m_List;

private:
	int m_Count;
	int m_Size;
};

CTaxRegister::CTaxRegister (void) {
	m_Count = 0;
}

CTaxRegister::~CTaxRegister (void) {}

bool CompareCitizens (const CCitizen & a, const CCitizen & b) {
	int diff = 0;
	diff = a.m_Name.compare(b.m_Name);

	if (diff < 0)
		return true;
	else if (diff == 0) {
		return (a.m_Addr.compare(b.m_Addr) < 0) ? true : false;
	}
	else
		return false;
}

bool CTaxRegister::Birth (const string & name, const string & addr, const string & account) {
	for (int i = 0; i < (int) m_List.size(); i++)
		if (m_List.at(i).m_Account == account)
			return false;

	CCitizen c (name, addr, account);
	int at = 0;
	auto pos = lower_bound(m_List.begin(), m_List.end(), c, CompareCitizens);
	at = pos - m_List.begin();

	if (at < m_Count && m_List.at(at) == c)
		return false;
	else
		m_List.insert(pos, c);

	m_Count++;

	return true;
}

void CTaxRegister::Print (void) {
	for (const auto & it : m_List)
		it.Print();
	cout << "-----------------------------------------------------" << endl;
}

bool CTaxRegister::Death (const string & name, const string & addr) {
	CCitizen c (name, addr);
	int at = 0;
	auto pos = lower_bound(m_List.begin(), m_List.end(), c, CompareCitizens);
	if (pos == m_List.end())
		return false;
	at = pos - m_List.begin();
	CCitizen a = m_List.at(at);

	if (at < m_Count && a == c) {
		m_List.erase(pos);
		// a.Print();
		return true;
	}
	else
		return false;
}

bool CTaxRegister::Income (const string & name, const string & addr, int amount) {
	CCitizen c (name, addr);
	int at = 0;
	auto pos = lower_bound(m_List.begin(), m_List.end(), c, CompareCitizens);
	if (pos == m_List.end())
		return false;
	at = pos - m_List.begin();
	CCitizen a = m_List.at(at);

	if (at < m_Count && a == c) {
		m_List.at(at).m_Income += amount;
		return true;
	}
	else
		return false;
}

bool CTaxRegister::Income (const string & account, int amount) {
	for (int i = 0; i < (int) m_List.size(); i++) {
		if (m_List.at(i).m_Account == account) {
			m_List.at(i).m_Income += amount;
			return true;
		}
	}
	
	return false;
}

bool CTaxRegister::Expense (const string & name, const string & addr, int amount) {
	CCitizen c (name, addr);
	int at = 0;
	auto pos = lower_bound(m_List.begin(), m_List.end(), c, CompareCitizens);
	if (pos == m_List.end())
		return false;
	at = pos - m_List.begin();
	CCitizen a = m_List.at(at);

	if (at < m_Count && a == c) {
		m_List.at(at).m_Expense += amount;
		return true;
	}
	else
		return false;
}

bool CTaxRegister::Expense (const string & account, int amount) {
	for (int i = 0; i < (int) m_List.size(); i++) {
		if (m_List.at(i).m_Account == account) {
			m_List.at(i).m_Expense += amount;
			return true;
		}
	}
	
	return false;
}

bool CTaxRegister::Audit (const string & name, const string & addr, string & account, int & sumIncome, int & sumExpense) const {
	CCitizen c (name, addr);
	int at = 0;
	auto pos = lower_bound(m_List.begin(), m_List.end(), c, CompareCitizens);
	if (pos == m_List.end())
		return false;
	at = pos - m_List.begin();
	CCitizen a = m_List.at(at);

	if (at < m_Count && a == c) {
		account = a.m_Account;
		sumIncome = a.m_Income;
		sumExpense = a.m_Expense;
		return true;
	}
	else
		return false;	
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] ) {
	bool   status;
string acct;
int    sumIncome, sumExpense;

CTaxRegister b1;
status = b1 . Birth ( "John Smith", "Oak Road 23", "123/456/789" );
cout << ((status) ? "true" : "false") << endl;
// b1.Print();
// status = true
status = b1 . Birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b1 . Birth ( "Peter Hacker", "Main Street 17", "634oddT" );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b1 . Birth ( "John Smith", "Main Street 17", "Z343rwZ" );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b1 . Income ( "Xuj5#94", 1000 );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b1 . Income ( "634oddT", 2000 );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b1 . Income ( "123/456/789", 3000 );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b1 . Income ( "634oddT", 4000 );
cout << ((status) ? "true" : "false") << endl;
// status = true
b1.Print();
status = b1 . Income ( "Peter Hacker", "Main Street 17", 2000 );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b1 . Expense ( "Jane Hacker", "Main Street 17", 2000 );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b1 . Expense ( "John Smith", "Main Street 17", 500 );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b1 . Expense ( "Jane Hacker", "Main Street 17", 1000 );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b1 . Expense ( "Xuj5#94", 1300 );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b1 . Audit ( "John Smith", "Oak Road 23", acct, sumIncome, sumExpense );
cout << ((status) ? "true" : "false") << " " << acct << " " << sumIncome << " " << sumExpense << endl;
// status = true, acct = "123/456/789", sumIncome=3000, sumExpense=0
status = b1 . Audit ( "Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense );
cout << ((status) ? "true" : "false") << " " << acct << " " << sumIncome << " " << sumExpense << endl;
// status = true, acct = "Xuj5#94", sumIncome=1000, sumExpense=4300
status = b1 . Audit ( "Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense );
cout << ((status) ? "true" : "false") << " " << acct << " " << sumIncome << " " << sumExpense << endl;
// status = true, acct = "634oddT", sumIncome=8000, sumExpense=0
status = b1 . Audit ( "John Smith", "Main Street 17", acct, sumIncome, sumExpense );
cout << ((status) ? "true" : "false") << " " << acct << " " << sumIncome << " " << sumExpense << endl;
// status = true, acct = "Z343rwZ", sumIncome=0, sumExpense=500
status = b1 . Death ( "John Smith", "Main Street 17" );
cout << ((status) ? "true" : "false") << endl;
// status = true

cout << "------------------------------------------------------------------" << endl;

CTaxRegister b2;
status = b2 . Birth ( "John Smith", "Oak Road 23", "123/456/789" );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b2 . Birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b2 . Income ( "634oddT", 4000 );
cout << ((status) ? "true" : "false") << endl;
// status = false
status = b2 . Expense ( "John Smith", "Main Street 18", 500 );
cout << ((status) ? "true" : "false") << endl;
// status = false
status = b2 . Audit ( "John Nowak", "Second Street 23", acct, sumIncome, sumExpense );
cout << ((status) ? "true" : "false") << endl;
// status = false
status = b2 . Death ( "Peter Nowak", "5-th Avenue" );
cout << ((status) ? "true" : "false") << endl;
// status = false
status = b2 . Birth ( "Jane Hacker", "Main Street 17", "4et689A" );
cout << ((status) ? "true" : "false") << endl;
// status = false
status = b2 . Birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" );
cout << ((status) ? "true" : "false") << endl;
// status = false
status = b2 . Death ( "Jane Hacker", "Main Street 17" );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b2 . Birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" );
cout << ((status) ? "true" : "false") << endl;
// status = true
status = b2 . Audit ( "Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense );
// cout << ((status) ? "true" : "false") << endl;
cout << ((status) ? "true" : "false") << " " << acct << " " << sumIncome << " " << sumExpense << endl;
// status = true, acct = "Xuj5#94", sumIncome=0, sumExpense=0
status = b2 . Birth ( "Joe Hacker", "Elm Street 23", "AAj5#94" );
cout << ((status) ? "true" : "false") << endl;
// status = false

	return 0;
}
#endif /* __PROGTEST__ */
