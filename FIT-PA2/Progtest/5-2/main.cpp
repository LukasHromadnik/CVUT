#ifndef __PROGTEST__
#include <cstring>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////

class CTimeStamp {
public:
	CTimeStamp (int year, int month, int day, int hour, int minute, int sec);
	CTimeStamp (const CTimeStamp & a);
	int Compare (const CTimeStamp & x) const;
	friend ostream & operator << (ostream & os, const CTimeStamp & x);
private:
	//
	int m_Time[6];
};

CTimeStamp::CTimeStamp (int year, int month, int day, int hour, int minute, int sec) {
	m_Time[0] = year;
	m_Time[1] = month;
	m_Time[2] = day;
	m_Time[3] = hour;
	m_Time[4] = minute;
	m_Time[5] = sec;
}

CTimeStamp::CTimeStamp (const CTimeStamp & a) {
	for (int i = 0; i < 6; i++)
		m_Time[i] = a.m_Time[i];
}

int CTimeStamp::Compare (const CTimeStamp & x) const {
	for (int i = 0; i < 6; i++) {
		if (m_Time[i] > x.m_Time[i])
			return 1;
		else if (m_Time[i] < x.m_Time[i])
			return -1;
	}

	return 0;
}

ostream & operator << (ostream & os, const CTimeStamp & x) {
	os << x.m_Time[0] << "-";

	if (x.m_Time[1] < 10)
		os << "0";
	os << x.m_Time[1] << "-";

	if (x.m_Time[2] < 10)
		os << "0";
	os << x.m_Time[2] << " ";

	if (x.m_Time[3] < 10)
		os << "0";
	os << x.m_Time[3] << ":";

	if (x.m_Time[4] < 10)
		os << "0";
	os << x.m_Time[4] << ":";

	if (x.m_Time[5] < 10)
		os << "0";
	os << x.m_Time[5];

	return os;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

class CMailBody {
public:
	CMailBody (int size, const char * data);
	friend ostream & operator << (ostream & os, const CMailBody & x) {return os << "mail body: " << x . m_Size << " B";}
	CMailBody (const CMailBody & a);
private:
	int m_Size;
	char * m_Data;
};

CMailBody::CMailBody (int size, const char * data) {
	m_Size = size;
	m_Data = new char [size + 1];
	strcpy(m_Data, data);
}

CMailBody::CMailBody (const CMailBody & a) {
	m_Size = a.m_Size;
	m_Data = a.m_Data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

class CAttach {
public:
	CAttach (int x) : m_X (x), m_RefCnt (1) { }
	void AddRef (void) const { m_RefCnt ++; }
	void Release (void) const { if (!--m_RefCnt) delete this; }
private:
	int m_X;
	mutable int m_RefCnt;
	CAttach (const CAttach & x);
	CAttach & operator = (const CAttach & x);
	~CAttach (void) { }
	friend ostream & operator << (ostream & os, const CAttach & x) {return os << "attachment: " << x . m_X << " B";}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////

#endif /*__PROGTEST__*/

///////////////////////////////////////////////////////////////////////////////////////////////////////

class CMail {
public:
	CMail (const CTimeStamp & timeStamp, const string & from, const CMailBody & body, const CAttach * attach);
	const string & From (void) const;
	const CMailBody & Body (void) const;
	const CTimeStamp & TimeStamp (void) const;
	const CAttach* Attachment (void) const;
	friend ostream & operator << (ostream & os, const CMail & x);
	bool operator < (const CMail & x) const;
private:
	CTimeStamp m_TimeStamp;
	string m_From;
	CMailBody m_Body;
	const CAttach * m_Attach;
};

CMail::CMail (const CTimeStamp & timeStamp, const string & from, const CMailBody & body, const CAttach * attach) : m_TimeStamp (timeStamp), m_Body (body) {
	m_From = from;
	m_Attach = attach;
	if (m_Attach)
		( * m_Attach).AddRef();
}

const string & CMail::From (void) const {
	return m_From;
}

const CMailBody & CMail::Body (void) const {
	return m_Body;
}

const CTimeStamp & CMail::TimeStamp (void) const {
	return m_TimeStamp;
}

const CAttach * CMail::Attachment (void) const {
	( * m_Attach).AddRef();
	return m_Attach;
}

bool CMail::operator < (const CMail & x) const {
	return (m_TimeStamp.Compare(x.m_TimeStamp) < 0);
}

ostream & operator << (ostream & os, const CMail & x) {
	os << x.m_TimeStamp << " " << x.m_From << " " << x.m_Body;

	if (x.m_Attach)
		os << " + " << ( * x.m_Attach);

	return os;
}

class CFrom {
public:
	CFrom (const CMail & a);
	bool operator < (const CFrom & a) const;
	CTimeStamp & TimeStamp (void);
	string From (void);
private:
	CTimeStamp m_TimeStamp;
	string m_From;
};

CFrom::CFrom (const CMail & a) : m_TimeStamp (a.TimeStamp()) {
	m_From = a.From();
}

bool CFrom::operator < (const CFrom & a) const {
	return ((m_TimeStamp).Compare(a.m_TimeStamp) < 0);
}

CTimeStamp & CFrom::TimeStamp (void) {
	return m_TimeStamp;
}

string CFrom::From(void) {
	return m_From;
}

class CMailBox {
public:
	CMailBox (void);
	bool Delivery (const CMail & mail);
	bool NewFolder (const string & folderName);
	bool MoveMail (const string & fromFolder, const string & toFolder);
	list<CMail> ListMail (const string & folderName, const CTimeStamp & from, const CTimeStamp & to);
	set<string> ListAddr (const CTimeStamp & from, const CTimeStamp & to) const;
private:
	map<string, vector<CMail> > m_List;
	vector<string> m_Folders;
	vector<CFrom> m_From;
};

CMailBox::CMailBox (void) {
	m_Folders.push_back("inbox");
}

bool CMailBox::Delivery (const CMail & mail) {
	if (m_List.count("inbox")) {
		auto it = lower_bound(m_List["inbox"].begin(), m_List["inbox"].end(), mail);
		m_List["inbox"].insert(it, mail);
	}
	else {
		vector<CMail> tmp;
		tmp.push_back(mail);
		m_List["inbox"] = tmp;
	}

	CFrom tmp (mail);
	m_From.push_back(tmp);

	return true;
}

list<CMail> CMailBox::ListMail (const string & folderName, const CTimeStamp & from, const CTimeStamp & to) {
	list<CMail> res;
	if ( ! m_List.count(folderName))
		return res;

	CMail a (from, "user1@fit.cvut.cz", CMailBody ( 14, "mail content 1" ), NULL);

	for (auto it = lower_bound(m_List[folderName].begin(), m_List[folderName].end(), a); it < m_List[folderName].end(); it++) {
		if (it->TimeStamp().Compare(to) <= 0)
			res.push_back( * it);
	}

	return res;
}

set<string> CMailBox::ListAddr (const CTimeStamp & from, const CTimeStamp & to) const {
	set<string> res;

	for (auto it : m_From)
		if (it.TimeStamp().Compare(from) >= 0 && it.TimeStamp().Compare(to) <= 0)
			res.insert(it.From());

	return res;
}

bool CMailBox::NewFolder (const string & folderName) {
	for (auto it : m_Folders)
		if (it == folderName)
			return false;

	m_Folders.push_back(folderName);

	return true;
}

bool CMailBox::MoveMail (const string & fromFolder, const string & toFolder) {
	bool from = false, to = false;

	for (auto it : m_Folders) {
		if (from && to)
			break;
		if (it == fromFolder)
			from = true;
		if (it == toFolder)
			to = true;
	}

	if ( ! from || ! to)
		return false;

	int sizeFrom = m_List[fromFolder].size();
	int sizeTo = m_List[toFolder].size();
	int max = sizeTo + sizeFrom;
	int it1 = 0;
	int it2 = 0;
	vector<CMail> tmp;

	for (int i = 0; i < max; i++) {
		
		if (it2 == sizeTo) {
			tmp.push_back(m_List[fromFolder].at(it1++));
			continue;
		}
		
		if (it1 == sizeFrom) {
			tmp.push_back(m_List[toFolder].at(it2++));
			continue;
		}

		if (m_List[fromFolder].at(it1) < m_List[toFolder].at(it2))
			tmp.push_back(m_List[fromFolder].at(it1++));
		else
			tmp.push_back(m_List[toFolder].at(it2++));

	}

	m_List[toFolder] = tmp;

	m_List.erase(fromFolder);

	return true;
}


#ifndef __PROGTEST__
void showMail (const list<CMail> & l) {
	for (list<CMail>::const_iterator it = l . begin (); it != l . end (); ++it)
		cout << *it << endl;
}

void showUsers (const set<string> & s) {
	for (set<string>::const_iterator it = s . begin (); it != s . end (); ++it)
		cout << *it << endl;
}

int main (void) {
	bool        st;
	// list<CMail> mailList;
	// set<string> users;
	CAttach   * att;
	CMailBox m0;
	
	st = m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 24, 13 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 1" ), NULL ) );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	// st = true

	st = m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 26, 23 ), "user2@fit.cvut.cz", CMailBody ( 22, "some different content" ), NULL ) );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	// st = true

	att = new CAttach ( 200 );

	st = m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 11, 23, 43 ), "boss1@fit.cvut.cz", CMailBody ( 14, "urgent message" ), att ) );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	// st = true

	st = m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 18, 52, 27 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 2" ), att ) );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	// st = true

	att -> Release ();

	att = new CAttach ( 97 );

	st = m0 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 16, 12, 48 ), "boss1@fit.cvut.cz", CMailBody ( 24, "even more urgent message" ), att ) );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	// st = true

	att -> Release ();

	cout << "------------------------------------------" << endl;
	showMail ( m0 . ListMail ( "inbox",
	                           CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
	                           CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) );
	// /*
	// ---8<---8<---8<---8<---8<---8<---
	// 2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B + attachment: 200 B
	// 2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B
	// 2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B
	// 2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B
	// 2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 200 B
	// ---8<---8<---8<---8<---8<---8<---
	// */
	cout << "------------------------------------------" << endl;
	showMail ( m0 . ListMail ( "inbox",
	                           CTimeStamp ( 2014, 3, 31, 15, 26, 23 ),
	                           CTimeStamp ( 2014, 3, 31, 16, 12, 48 ) ) );
	// /*
	// ---8<---8<---8<---8<---8<---8<---
	// 2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B
	// 2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B + attachment: 97 B
	// ---8<---8<---8<---8<---8<---8<---
	// */
	cout << "------------------------------------------" << endl;
	showUsers ( m0 . ListAddr ( CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
	                            CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) );
	// /*
	// ---8<---8<---8<---8<---8<---8<---
	// boss1@fit.cvut.cz
	// user1@fit.cvut.cz
	// user2@fit.cvut.cz
	// ---8<---8<---8<---8<---8<---8<---
	// */
	cout << "------------------------------------------" << endl;
	showUsers ( m0 . ListAddr ( CTimeStamp ( 2014, 3, 31, 15, 26, 23 ),
	                            CTimeStamp ( 2014, 3, 31, 16, 12, 48 ) ) );
	// /*
	// ---8<---8<---8<---8<---8<---8<---
	// boss1@fit.cvut.cz
	// user2@fit.cvut.cz
	// ---8<---8<---8<---8<---8<---8<---
	// */
	
	CMailBox m1;
	
	st = m1 . NewFolder ( "work" );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	 // st = true

	st = m1 . NewFolder ( "spam" );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	 // st = true

	st = m1 . NewFolder ( "spam" );
	if (st != false) cout << "Chyba: " << __LINE__ << endl;
	 // st = false

	st = m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 24, 13 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 1" ), NULL ) );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	 // st = true

	att = new CAttach ( 500 );

	st = m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 15, 26, 23 ), "user2@fit.cvut.cz", CMailBody ( 22, "some different content" ), att ) );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	 // st = true
	
	att -> Release ();
	
	st = m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 11, 23, 43 ), "boss1@fit.cvut.cz", CMailBody ( 14, "urgent message" ), NULL ) );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	 // st = true

	att = new CAttach ( 468 );
	
	st = m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 18, 52, 27 ), "user1@fit.cvut.cz", CMailBody ( 14, "mail content 2" ), att ) );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	 // st = true

	att -> Release ();
	
	st = m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 16, 12, 48 ), "boss1@fit.cvut.cz", CMailBody ( 24, "even more urgent message" ), NULL ) );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	 // st = true

	cout << "------------------------------------------" << endl;
	showMail ( m1 . ListMail ( "inbox",
	                           CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
	                           CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) );
	
	// ---8<---8<---8<---8<---8<---8<---
	// 2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B
	// 2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B
	// 2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B
	// 2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B
	// 2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B
	// ---8<---8<---8<---8<---8<---8<---
	cout << "------------------------------------------" << endl;
	showMail ( m1 . ListMail ( "work",
	                           CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
	                           CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) );
	// /*
	// ---8<---8<---8<---8<---8<---8<---
	// ---8<---8<---8<---8<---8<---8<---
	// */
	
	st = m1 . MoveMail ( "inbox", "work" );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	 // st = true
	
	cout << "------------------------------------------" << endl;
	showMail ( m1 . ListMail ( "inbox",
	                           CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
	                           CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) );
	// /*
	// ---8<---8<---8<---8<---8<---8<---
	// ---8<---8<---8<---8<---8<---8<---
	// */
	cout << "------------------------------------------" << endl;
	showMail ( m1 . ListMail ( "work",
	                           CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
	                           CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) );
	// /*
	// ---8<---8<---8<---8<---8<---8<---
	// 2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B
	// 2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B
	// 2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B
	// 2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B
	// 2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B
	// ---8<---8<---8<---8<---8<---8<---
	// */
	
	st = m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 19, 24, 13 ), "user2@fit.cvut.cz", CMailBody ( 14, "mail content 4" ), NULL ) );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	 // st = true

	att = new CAttach ( 234 );
	
	st = m1 . Delivery ( CMail ( CTimeStamp ( 2014, 3, 31, 13, 26, 23 ), "user3@fit.cvut.cz", CMailBody ( 9, "complains" ), att ) );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	 // st = true

	att -> Release ();
	
	cout << "------------------------------------------" << endl;
	showMail ( m1 . ListMail ( "inbox",
	                           CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
	                           CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) );
	// /*
	// ---8<---8<---8<---8<---8<---8<---
	// 2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B
	// 2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B
	// ---8<---8<---8<---8<---8<---8<---
	// */
	cout << "------------------------------------------" << endl;
	showMail ( m1 . ListMail ( "work",
	                           CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
	                           CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) );
	// /*
	// ---8<---8<---8<---8<---8<---8<---
	// 2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B
	// 2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B
	// 2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B
	// 2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B
	// 2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B
	// ---8<---8<---8<---8<---8<---8<---
	// */

	st = m1 . MoveMail ( "inbox", "work" );
	if (st != true) cout << "Chyba: " << __LINE__ << endl;
	 // st = true

	cout << "------------------------------------------" << endl;
	showMail ( m1 . ListMail ( "inbox",
	                           CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
	                           CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) );
	// /*
	// ---8<---8<---8<---8<---8<---8<---
	// ---8<---8<---8<---8<---8<---8<---
	// */
	cout << "------------------------------------------" << endl;
	showMail ( m1 . ListMail ( "work",
	                           CTimeStamp ( 2000, 1, 1, 0, 0, 0 ),
	                           CTimeStamp ( 2050, 12, 31, 23, 59, 59 ) ) );
	// /*
	// ---8<---8<---8<---8<---8<---8<---
	// 2014-03-31 11:23:43 boss1@fit.cvut.cz mail body: 14 B
	// 2014-03-31 13:26:23 user3@fit.cvut.cz mail body: 9 B + attachment: 234 B
	// 2014-03-31 15:24:13 user1@fit.cvut.cz mail body: 14 B
	// 2014-03-31 15:26:23 user2@fit.cvut.cz mail body: 22 B + attachment: 500 B
	// 2014-03-31 16:12:48 boss1@fit.cvut.cz mail body: 24 B
	// 2014-03-31 18:52:27 user1@fit.cvut.cz mail body: 14 B + attachment: 468 B
	// 2014-03-31 19:24:13 user2@fit.cvut.cz mail body: 14 B
	// ---8<---8<---8<---8<---8<---8<---
	// */
 
 	cout << "------------------------------------------" << endl;
	showMail ( m1 . ListMail ( "work",
	                           CTimeStamp ( 2014, 3, 31, 11, 23, 40 ),
	                           CTimeStamp ( 2014, 3, 31, 11, 23, 41 ) ) );

	cout << "------------------------------------------" << endl;
	
	return 0;
}

#endif /* __PROGTEST__ */
