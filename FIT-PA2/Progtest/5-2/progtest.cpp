#include <iostream>

using namespace std;

class CTimeStamp {
public:
	CTimeStamp (int year, int month, int day, int hour, int minute, int sec);
	friend ostream & operator << (ostream & os, const CTimeStamp & x);
	void operator = (const CTimeStamp & x);
	int Compare (const CTimeStamp & x) const;
protected:
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

int CTimeStamp::Compare (const CTimeStamp & x) const {
	for (int i = 0; i < 6; i++) {
		if (m_Time[i] < x.m_Time[i])
			return -1;
		else if (m_Time[i] > x.m_Time[i])
			return 1;
	}

	return 0;
}

void CTimeStamp::operator = (const CTimeStamp & x) {
	for (int i = 0; i < 6; i++)
		m_Time[i] = x.m_Time[i];
}

ostream & CTimeStamp::operator << (ostream & os, const CTimeStamp & x) {
	os << x.m_Time[0] << "-" << x.m_Time[1] << "-" << x.m_Time[2] << " " << x.m_Time[3] << ":" << x.m_Time[4] << ":" << x.m_Time[5];

	return os;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CMailBody {
public:
	CMailBody (int size, const char * data);
	~CMailBody (void);
	friend ostream & operator << (ostream & os, const CMailBody & x) { return os << "mail body: " << x . m_Size << " B"; }
private:
	int m_Size;
	char * m_Data;
};

CMailBody::CMailBody (int size, const char * data) {
	m_Size = size;
	m_Data = new char[size];
	strcpy(m_Data, data);
}

CMailBody::~CMailBody (void) {
	delete [] m_Data;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CAttach {
public:
	CAttach (int x) : m_X (x), m_RefCnt (1) { }
	void AddRef (void) const { m_RefCnt ++; }
	void Release (void) const { if (! --m_RefCnt) delete this; }
private:
	int m_X;
	mutable int m_RefCnt;
	CAttach (const CAttach & x);
	CAttach & operator = (const CAttach & x);
	~CAttach (void) { }
	friend ostream & operator << (ostream & os, const CAttach & x) { return os << "attachment: " << x . m_X << " B"; }
};
