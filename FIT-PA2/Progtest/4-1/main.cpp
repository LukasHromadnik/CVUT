#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
#endif /* __PROGTEST__ */

class CStr {
public:
	CStr (const char *);
	CStr (const CStr &);
	~CStr (void);
	void operator = (const char *);
	void operator = (const CStr &);
	friend ostream & operator << (ostream &, const CStr &);
	int GetLength (void);
	bool Equal (const char *);
	bool Equal (const CStr &);
private:
	char * m_Str;
	int m_Length;
};

CStr::CStr (const char * a) {
	(*this) = a;
}

CStr::CStr (const CStr &a) {
	(*this) = a;
}

void CStr::operator = (const char * a) {
	m_Length = strlen(a);
	m_Str = new char [m_Length + 1];
	strcpy (m_Str, a);
}

void CStr::operator = (const CStr &a) {
	m_Length = a.m_Length;
	m_Str = new char [m_Length + 1];
	strcpy (m_Str, a.m_Str);
}

ostream & operator << (ostream & os, const CStr & a) {
	os << a.m_Str;

	return os;
}

CStr::~CStr (void) {
	delete [] m_Str;
}

int CStr::GetLength (void) {
	return m_Length;
}

bool CStr::Equal (const char * a) {
	return strcmp(m_Str, a) == 0;
}

bool CStr::Equal (const CStr & a) {
	return strcmp(m_Str, a.m_Str) == 0;
}

///////////////////////////////////////////////////////////////////////////////////////

class CTransaction {
public:
	CTransaction (const char * acc, int amount, const char * signature, int sign);
	~CTransaction (void);

	void Remove (void);
	CTransaction * Copy (void);

	friend ostream & operator << (ostream & os, const CTransaction & a);

	CStr m_AccID;
	int m_Amount;
	CStr m_Signature;
	int m_Sign;

	int m_Used;
};

CTransaction::CTransaction (const char * acc, int amount, const char * signature, int sign) : m_AccID (acc), m_Signature (signature) {
	m_Amount = amount;
	m_Sign = sign;
	m_Used = 1;
}

CTransaction::~CTransaction (void) {
}

void CTransaction::Remove (void) {
	if (m_Used == 1)
		delete this;
	else
		m_Used--;
}

CTransaction * CTransaction::Copy (void) {
	m_Used++;

	return this;
}

ostream & operator << (ostream & os, const CTransaction & a) {
	os << ((a.m_Sign) ? " + " : " - ") << a.m_Amount << ", ";
	os << ((a.m_Sign) ? "from: " : "to: ") << a.m_AccID << ", ";
	os << "sign: " << a.m_Signature << ", ";
	os << "used: " << a.m_Used << endl;

	return os;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

class CAccount {
public:
	CAccount (const char * accID, int initialBalance);
	CAccount (CAccount & a);
	~CAccount (void);
	void Remove (void);
	CAccount * Copy (void);
	void NewInstance (void);
	CAccount * GetRightAccount (void);
	void Transaction (const char * acc, int amount, const char * signature, int sign);
	bool Equal (const char * accID);
	int Balance (void);
	bool Trim (void);
	friend ostream & operator << (ostream & os, const CAccount & a);
	void operator += (int amount);
	void operator -= (int amount);

private:
	CStr m_AccID;
	int m_InitialBalance;
	int m_Balance;
	CTransaction ** m_List;
	int m_TransactionsCount;
	int m_ListSize;
	int m_Used;
};

CAccount::CAccount (const char * accID, int initialBalance) : m_AccID (accID) {
	m_InitialBalance = m_Balance = initialBalance;
	m_TransactionsCount = 0;
	m_ListSize = 100;
	m_List = new CTransaction * [m_ListSize];
	m_Used = 1;
}

CAccount::CAccount (CAccount & a) : m_AccID (a.m_AccID) {
	m_InitialBalance = a.m_InitialBalance;
	m_Balance = a.m_Balance;
	m_TransactionsCount = a.m_TransactionsCount;
	m_ListSize = a.m_ListSize;
	m_List = new CTransaction * [m_ListSize];
	for (int i = 0; i < m_TransactionsCount; i++)
		m_List[i] = a.m_List[i]->Copy();
	a.NewInstance();
	m_Used = 1;
}

CAccount::~CAccount (void) {
	for (int i = 0; i < m_TransactionsCount; i++)
		m_List[i]->Remove();

	delete [] m_List;
}

void CAccount::Remove (void) {
	if (m_Used == 1)
		delete this;
	else
		m_Used--;
}

CAccount * CAccount::Copy (void) {
	m_Used++;

	return this;
}

void CAccount::NewInstance (void) {
	if (m_Used > 1) m_Used--;
}

CAccount * CAccount::GetRightAccount (void) {
	return (m_Used == 1) ? this : new CAccount ( * this);
}

void CAccount::Transaction (const char * acc, int amount, const char * signature, int sign) {
	if ((m_TransactionsCount + 1) == m_ListSize) {
		CTransaction ** tmp = new CTransaction * [m_ListSize * 2];
		for (int i = 0; i < m_ListSize; i++)
			tmp[i] = m_List[i];
		delete [] m_List;
		m_List = tmp;
		m_ListSize *= 2;
	}

	m_List[m_TransactionsCount++] = new CTransaction (acc, amount, signature, sign);
}

bool CAccount::Equal (const char * accID) {
	return m_AccID.Equal(accID);
}

int CAccount::Balance (void) {
	return m_Balance;
}

bool CAccount::Trim (void) {
	for (int i = 0; i < m_TransactionsCount; i++)
		m_List[i]->Remove();
	delete [] m_List;
	m_TransactionsCount = 0;
	m_ListSize = 100;
	m_List = new CTransaction * [m_ListSize];

	m_InitialBalance = m_Balance;

	return true;
}

void CAccount::operator += (int amount) {
	m_Balance += amount;
}

void CAccount::operator -= (int amount) {
	m_Balance -= amount;
}

ostream & operator << (ostream & os, const CAccount & a) {
	os << a.m_AccID << ":" << endl;
	os << "   " << a.m_InitialBalance << endl;
	for (int i = 0; i < a.m_TransactionsCount; i++)
		os << * a.m_List[i];
	os << " = " << a.m_Balance << endl;
	os << "used: " << a.m_Used << endl;

	return os;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

class CBank {
public:
	CBank (void);
	CBank (const CBank & a);
	~CBank ();
	void operator = (const CBank & a);
	bool NewAccount (const char * accID, int initialBalance);
	bool Transaction (const char * debAccID, const char * credAccID, int amount, const char * signature);
	bool TrimAccount (const char * accID);
	CAccount & Account (const char * accID);
private:
	CAccount ** m_List;
	int m_AccountsCount;
	int m_ListSize;
};

CBank::CBank (void) {
	m_ListSize = 100;
	m_List = new CAccount * [m_ListSize];
	m_AccountsCount = 0;
}

CBank::CBank (const CBank & a) {
	m_ListSize = a.m_ListSize;
	m_AccountsCount = a.m_AccountsCount;
	m_List = new CAccount * [m_ListSize];

	for (int i = 0; i < m_AccountsCount; i++)
		m_List[i] = a.m_List[i]->Copy();
}

CBank::~CBank (void) {
	for (int i = 0; i < m_AccountsCount; i++)
		m_List[i]->Remove();

	delete [] m_List;
}

void CBank::operator = (const CBank & a) {
	m_ListSize = a.m_ListSize;
	m_AccountsCount = a.m_AccountsCount;
	m_List = new CAccount * [m_ListSize];

	for (int i = 0; i < m_AccountsCount; i++)
		m_List[i] = a.m_List[i]->Copy();
}

bool CBank::NewAccount (const char * accID, int initialBalance) {
	for (int i = 0; i < m_AccountsCount; i++)
		if (m_List[i]->Equal(accID))
			return false;

	if ((m_AccountsCount + 1) == m_ListSize) {
		CAccount ** tmp = new CAccount * [m_ListSize * 2];
		for (int i = 0; i < m_ListSize; i++)
			tmp[i] = m_List[i];
		delete [] m_List;
		m_List = tmp;
		m_ListSize *= 2;
	}

	m_List[m_AccountsCount++] = new CAccount (accID, initialBalance);

	return true;
}

bool CBank::Transaction (const char * debAccID, const char * credAccID, int amount, const char * signature) {
	if (strcmp(debAccID, credAccID) == 0)
		return false;

	int foundDeb = 0, foundCred = 0, deb, cred;

	for (int i = 0; i < m_AccountsCount; i++) {
		
		if (foundDeb && foundCred)
			break;
		
		if ( ! foundDeb && m_List[i]->Equal(debAccID)) {
			deb = i;
			foundDeb = 1;
			continue;
		}
		
		if ( ! foundCred && m_List[i]->Equal(credAccID)) {
			cred = i;
			foundCred = 1;
			continue;
		}
	}
	
	if ( ! foundDeb || ! foundCred)
		return false;

	m_List[deb] = m_List[deb]->GetRightAccount();
	m_List[deb]->Transaction(credAccID, amount, signature, 0);
	(* m_List[deb]) -= amount;
	m_List[cred] = m_List[cred]->GetRightAccount();
	m_List[cred]->Transaction(debAccID, amount, signature, 1);
	(* m_List[cred]) += amount;

	return true;
}

bool CBank::TrimAccount (const char * accID) {
	int acc;
	int found = 0;
	
	for (int i = 0; i < m_AccountsCount; i++) {
		if (m_List[i]->Equal(accID)) {
			acc = i;
			found = 1;
			break;
		}
	}

	if ( ! found)
		return false;

	m_List[acc] = m_List[acc]->GetRightAccount();
	return m_List[acc]->Trim();
}

CAccount & CBank::Account (const char * accID) {
	for (int i = 0; i < m_AccountsCount; i++)
		if (m_List[i]->Equal(accID))
			return (* m_List[i]);

	throw "Account not found";
}

#ifndef __PROGTEST__
int main() {
		bool status;
	int balance = 0;
	
	CBank a;
	status = a . NewAccount ( "123456", 1000 );
	cout << "r: true; s: " << boolalpha << status << endl;
	status = a . NewAccount ( "987654", -500 );
	cout << "r: true; s: " << boolalpha << status << endl;
	status = a . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" );
	cout << "r: true; s: " << boolalpha << status << endl;
	status = a . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" );
	cout << "r: true; s: " << boolalpha << status << endl;
	status = a . NewAccount ( "111111", 5000 );
	cout << "r: true; s: " << boolalpha << status << endl;
	status = a . Transaction ( "111111", "987654", 290, "Okh6e+8rAiuT5=" );
	cout << "r: true; s: " << boolalpha << status << endl;
	balance = a . Account ( "123456" ). Balance ( );
	cout << "r: -2190; s: " << balance << endl;
	balance = a . Account ( "987654" ). Balance ( );
	cout << "r: 2980; s: " << balance << endl;
	balance = a . Account ( "111111" ). Balance ( );
	cout << "r: 4710; s: " << balance << endl;
	cout << a . Account ( "123456" );
	cout << a . Account ( "987654" );
	cout << a . Account ( "111111" );
	status = a . TrimAccount ( "987654" );
	cout << "r: true; s: " << boolalpha << status << endl;
	status = a . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" );
	cout << "r: true; s: " << boolalpha << status << endl;
	cout << a . Account ( "987654" );

	CBank c;
	char accCpy[100], debCpy[100], credCpy[100], signCpy[100];
	strncpy ( accCpy, "123456", sizeof ( accCpy ) );
	status = c . NewAccount ( accCpy, 1000 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	strncpy ( accCpy, "987654", sizeof ( accCpy ) );
	status = c . NewAccount ( accCpy, -500 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	strncpy ( debCpy, "123456", sizeof ( debCpy ) );
	strncpy ( credCpy, "987654", sizeof ( credCpy ) );
	strncpy ( signCpy, "XAbG5uKz6E=", sizeof ( signCpy ) );
	status = c . Transaction ( debCpy, credCpy, 300, signCpy );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	strncpy ( debCpy, "123456", sizeof ( debCpy ) );
	strncpy ( credCpy, "987654", sizeof ( credCpy ) );
	strncpy ( signCpy, "AbG5uKz6E=", sizeof ( signCpy ) );
	status = c . Transaction ( debCpy, credCpy, 2890, signCpy );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	strncpy ( accCpy, "111111", sizeof ( accCpy ) );
	status = c . NewAccount ( accCpy, 5000 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	strncpy ( debCpy, "111111", sizeof ( debCpy ) );
	strncpy ( credCpy, "987654", sizeof ( credCpy ) );
	strncpy ( signCpy, "Okh6e+8rAiuT5=", sizeof ( signCpy ) );
	status = c . Transaction ( debCpy, credCpy, 2890, signCpy );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	balance = c . Account ( "123456" ). Balance ( );
	cout << "r: -2190; s: " << balance << endl;
	 // balance = -2190
	balance = c . Account ( "987654" ). Balance ( );
	cout << "r: 5580; s: " << balance << endl;
	 // balance = 5580
	balance = c . Account ( "111111" ). Balance ( );
	cout << "r: 2110; s: " << balance << endl;
	 // balance = 2110
	cout << c . Account ( "123456" );
	/*
	---8<---8<---8<---8<---8<---8<---8<---
	123456:
	   1000
	 - 300, to: 987654, sign: XAbG5uKz6E=
	 - 2890, to: 987654, sign: AbG5uKz6E=
	 = -2190
	---8<---8<---8<---8<---8<---8<---8<---
	*/
	cout << c . Account ( "987654" );
	/*
	---8<---8<---8<---8<---8<---8<---8<---
	987654:
	   -500
	 + 300, from: 123456, sign: XAbG5uKz6E=
	 + 2890, from: 123456, sign: AbG5uKz6E=
	 + 2890, from: 111111, sign: Okh6e+8rAiuT5=
	 = 5580
	---8<---8<---8<---8<---8<---8<---8<---
	*/
	cout << c . Account ( "111111" );
	/*
	---8<---8<---8<---8<---8<---8<---8<---
	111111:
	   5000
	 - 2890, to: 987654, sign: Okh6e+8rAiuT5=
	 = 2110
	---8<---8<---8<---8<---8<---8<---8<---
	*/
	status = c . TrimAccount ( "987654" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	strncpy ( debCpy, "111111", sizeof ( debCpy ) );
	strncpy ( credCpy, "987654", sizeof ( credCpy ) );
	strncpy ( signCpy, "asdf78wrnASDT3W", sizeof ( signCpy ) );
	status = c . Transaction ( debCpy, credCpy, 123, signCpy );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	cout << c . Account ( "987654" );

	CBank e;
	status = e . NewAccount ( "123456", 1000 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = e . NewAccount ( "987654", -500 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = e . NewAccount ( "123456", 3000 );
	cout << "r: false; s: " << boolalpha << status << endl;
	 // status = false
	status = e . Transaction ( "123456", "666", 100, "123nr6dfqkwbv5" );
	cout << "r: false; s: " << boolalpha << status << endl;
	 // status = false
	status = e . Transaction ( "666", "123456", 100, "34dGD74JsdfKGH" );
	cout << "r: false; s: " << boolalpha << status << endl;
	 // status = false
	status = e . Transaction ( "123456", "123456", 100, "Juaw7Jasdkjb5" );
	cout << "r: false; s: " << boolalpha << status << endl;
	 // status = false
	try {
		balance = e . Account ( "666" ). Balance ( );
	 	// exception thrown
	}
	catch (const char * e) {
		cout << e << endl;
	}
	try {
		cout << e . Account ( "666" );
	 	// exception thrown
	}
	catch (const char * e) {
		cout << e << endl;
	}
	 // exception thrown
	status = e . TrimAccount ( "666" );
	cout << "r: false; s: " << boolalpha << status << endl;
	 // status = false

	CBank g;
	status = g . NewAccount ( "123456", 1000 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = g . NewAccount ( "987654", -500 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = g . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = g . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = g . NewAccount ( "111111", 5000 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = g . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	CBank h ( g );
	status = g . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = h . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = g . NewAccount ( "99999999", 7000 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = g . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = g . TrimAccount ( "111111" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = g . Transaction ( "123456", "111111", 221, "Q23wr234ER==" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	cout << g . Account ( "111111" );
	/*
	---8<---8<---8<---8<---8<---8<---8<---
	111111:
	   -1802
	 + 221, from: 123456, sign: Q23wr234ER==
	 = -1581
	---8<---8<---8<---8<---8<---8<---8<---
	*/
	cout << g . Account ( "99999999" );
	/*
	---8<---8<---8<---8<---8<---8<---8<---
	99999999:
	   7000
	 + 3789, from: 111111, sign: aher5asdVsAD
	 = 10789
	---8<---8<---8<---8<---8<---8<---8<---
	*/
	cout << g . Account ( "987654" );
	
	/*---8<---8<---8<---8<---8<---8<---8<---
	987654:
	   -500
	 + 300, from: 123456, sign: XAbG5uKz6E=
	 + 2890, from: 123456, sign: AbG5uKz6E=
	 + 2890, from: 111111, sign: Okh6e+8rAiuT5=
	 + 123, from: 111111, sign: asdf78wrnASDT3W
	 = 5703
	---8<---8<---8<---8<---8<---8<---8<---*/
	
	cout << h . Account ( "111111" );
	/*
	---8<---8<---8<---8<---8<---8<---8<---
	111111:
	   5000
	 - 2890, to: 987654, sign: Okh6e+8rAiuT5=
	 - 789, to: 987654, sign: SGDFTYE3sdfsd3W
	 = 1321
	---8<---8<---8<---8<---8<---8<---8<---
	*/
	try {
		cout << h . Account ( "99999999" );
	}
	catch (const char * e) {
		cout << e << endl;
	}
	 // exception thrown
	cout << h . Account ( "987654" );
	/*
	---8<---8<---8<---8<---8<---8<---8<---
	987654:
	   -500
	 + 300, from: 123456, sign: XAbG5uKz6E=
	 + 2890, from: 123456, sign: AbG5uKz6E=
	 + 2890, from: 111111, sign: Okh6e+8rAiuT5=
	 + 789, from: 111111, sign: SGDFTYE3sdfsd3W
	 = 6369
	---8<---8<---8<---8<---8<---8<---8<---
	*/

	CBank i;
	CBank j;
	status = i . NewAccount ( "123456", 1000 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = i . NewAccount ( "987654", -500 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = i . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = i . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = i . NewAccount ( "111111", 5000 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = i . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	j = i;
	status = i . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = j . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = i . NewAccount ( "99999999", 7000 );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = i . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	status = i . TrimAccount ( "111111" );
	cout << "r: true; s: " << boolalpha << status << endl;
	 // status = true
	cout << i . Account ( "111111" );
	/*
	---8<---8<---8<---8<---8<---8<---8<---
	111111:
	   -1802
	 = -1802
	---8<---8<---8<---8<---8<---8<---8<---
	*/
	cout << j . Account ( "111111" );
	/*
	---8<---8<---8<---8<---8<---8<---8<---
	111111:
	   5000
	 - 2890, to: 987654, sign: Okh6e+8rAiuT5=
	 - 789, to: 987654, sign: SGDFTYE3sdfsd3W
	 = 1321
	---8<---8<---8<---8<---8<---8<---8<---
	*/

}
#endif /* __PROGTEST__ */
