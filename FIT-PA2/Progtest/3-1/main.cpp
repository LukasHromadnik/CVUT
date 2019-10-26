#ifndef __PROGTEST__
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
#endif /*__PROGTEST__*/

class CBigInt {
public:
	CBigInt ();
	CBigInt (int x);
	CBigInt (const char * x);

	friend ostream & operator << (ostream & os, const CBigInt & a);
	friend istringstream & operator >> (istringstream & is, CBigInt & a);
	void operator = (int x);
	void operator = (const char * x);
	void operator = (const CBigInt & a);
	void operator += (int x);
	void operator += (const char * x);
	void operator += (const CBigInt & a);
	void operator *= (int x);
	void operator *= (const char * x);
	void operator *= (const CBigInt & a);
	bool operator == (int x);
	bool operator == (const char * x);
	bool operator == (const CBigInt & a);

	void operator -= (const CBigInt & a);
	bool operator > (const CBigInt & a);
	bool isNegative (void);

private:
	int m_NegativeSign;
	vector<int> m_Data;
};

string ZeroGenerator (int count) {
	string s;

	for (int i = 0; i < count; i++)
		s += "0";

	return s;
}

CBigInt operator + (const CBigInt & a, int x) {
	CBigInt tmp (a);
	tmp += x;
	return tmp;
}

CBigInt operator + (const CBigInt & a, const char * x) {
	CBigInt tmp (a);
	tmp += x;
	return tmp;
}

CBigInt operator * (const CBigInt & a, int x) {
	CBigInt tmp (a);
	tmp *= x;
	return tmp;
}

CBigInt operator * (const CBigInt & a, const char * x) {
	CBigInt tmp (a);
	tmp *= x;
	return tmp;
}

bool CBigInt::isNegative (void) {
	return m_NegativeSign;
}

CBigInt::CBigInt () {
	CBigInt(0);
}

CBigInt::CBigInt (int x) {
	if (x == 0) {
		m_Data.push_back(0);
		m_NegativeSign = 0;
		return;
	}

	m_NegativeSign = (x < 0) ? 1 : 0;

	if (x < 0)
		x *= -1;
	
	int modulo = 0;
	while (x > 0) {
		modulo = x % 10;
		m_Data.push_back(modulo);
		x /= 10;
	}
}

CBigInt::CBigInt (const char * x) {
	m_NegativeSign = 0;
	string s (x);
	int i = 0;

	if (s[0] == '-') {
		m_NegativeSign = 1;
		i++;
	}

	for ( ; i < (int) s.length(); i++)
		m_Data.insert(m_Data.begin(), (int) (s[i] - '0'));

}

ostream & operator << (ostream & os, const CBigInt & a) {
	if (a.m_NegativeSign)
		os << "-";
	
	for (int i = (int) a.m_Data.size(); i > 0; i--) {
		
		if (i == (int) a.m_Data.size() && i != 1 && a.m_Data[i - 1] == 0)
			continue;

		os << a.m_Data[i - 1];
	}

	// os << endl;

	return os;
}

istringstream & operator >> (istringstream & is, CBigInt & a) {
	string s(is.str());
	s.erase(0, s.find_first_not_of(' '));
	int i = 0;

	if (isdigit(s[i]) || s[i] == '-') {
		i++;

		for ( ; i < (int) s.length(); i++) {
			if ( ! isdigit(s[i]))
				break;
		}

		if (i != (int) s.length())
			s.erase(i, s.length() - 1);

		a = s.c_str();
	}
	else
		is.clear(ios::failbit);

	return is;
	
}

void CBigInt::operator = (int x) {
	CBigInt tmp (x);
	*this = tmp;
}

void CBigInt::operator = (const char * x) {
	CBigInt a (x);
	*this = a;
}

void CBigInt::operator = (const CBigInt & a) {
	// vynulovani m_Data
	vector<int> new_vector;
	m_Data = new_vector;

	// nastaveni znamenka
	m_NegativeSign = a.m_NegativeSign;

	// prekopirovani hodnot
	for (int i = 0; i < (int) a.m_Data.size(); i++)
		m_Data.push_back(a.m_Data[i]);
}

void CBigInt::operator += (int x) {
	CBigInt tmp (x);
	if (tmp.isNegative()) {
		// (-x) + (-y) = (-z)
		if ((*this).isNegative()) {
			operator += (tmp);
			(*this).m_NegativeSign = 1;
		}
		else {
			// x + (-y) > 0
			if (*this > tmp)
				operator -= (tmp);
			// x + (-y) < 0
			else {
				tmp -= *this;
				*this = tmp;
				(*this).m_NegativeSign = 1;
			}
		}
	}
	else {
		if ((*this).isNegative()) {
			// (-x) - y < 0
			if (*this > tmp) {
				operator -= (tmp);
				(*this).m_NegativeSign = 1;
			}
			// (-x) - y > 0
			else {
				tmp -= *this;
				*this = tmp;
			}
		}
		// x + y = z
		else
			operator += (tmp);
	}
}

void CBigInt::operator += (const char * x) {
	CBigInt tmp (x);
	if (tmp.isNegative()) {
		// (-x) + (-y) = (-z)
		if ((*this).isNegative()) {
			operator += (tmp);
			(*this).m_NegativeSign = 1;
		}
		else {
			// x + (-y) > 0
			if (*this > tmp)
				operator -= (tmp);
			// x + (-y) < 0
			else {
				tmp -= *this;
				*this = tmp;
				(*this).m_NegativeSign = 1;
			}
		}
	}
	else {
		if ((*this).isNegative()) {
			// (-x) - y < 0
			if (*this > tmp) {
				operator -= (tmp);
				(*this).m_NegativeSign = 1;
			}
			// (-x) - y > 0
			else {
				tmp -= *this;
				*this = tmp;
			}
		}
		// x + y = z
		else
			operator += (tmp);
	}
}

void CBigInt::operator += (const CBigInt & a) {
	CBigInt max, min;
	int carry = 0;
	int sum = 0;
	int value = 0;

	// nastaveni minima a maxima
	if (*this > a) {
		max = *this;
		min = a;
	}
	else {
		max = a;
		min = *this;
	}

	for (int i = 0; i < (int) max.m_Data.size(); i++) {
			
		// test preteceni mensiho cisla
		value = (i >= (int) min.m_Data.size()) ? 0 : min.m_Data[i];
		
		// scitacka
		sum = max.m_Data[i] + value + carry;

		// vypocet carry
		if (sum > 9) {
			carry = sum / 10;
			// carry = 0;
			
			sum %= 10;
		}
		else {
		
			// vynulovani carry
			carry = 0;
		}

		// ulozeni sumy
		max.m_Data[i] = sum;
	}

	// pokud je carry, prida se do vectoru
	if (carry > 0)
		max.m_Data.push_back(carry);

	// nahrazeni m_Data u puvodniho cisla
	m_Data = max.m_Data;
}

void CBigInt::operator -= (const CBigInt & a) {
	CBigInt max, min;
	int carry = 0;
	int m_X;
	int m_Y;
	int result_negative = 0;

	// nastaveni minima a maxima
	if (*this > a) {
		max = *this;
		min = a;
	}
	else {
		max = a;
		min = *this;
		result_negative = 1;
	}

	for (int i = 0; i < (int) max.m_Data.size(); i++) {
		
		// test preteceni mensiho cisla - dolni cislo
		m_Y = (i >= (int) min.m_Data.size()) ? 0 : min.m_Data[i];
		m_Y += carry;
		
		// pro zjednoduseni vypoctu - horni cislo
		m_X = max.m_Data[i];

		if (m_Y > m_X) {
			m_X += 10;
			carry = 1;
		}
		else
			carry = 0;

		// ulozeni sumy
		max.m_Data[i] = m_X - m_Y;
	}

	// nahrazeni m_Data u puvodniho cisla
	m_Data = max.m_Data;
	m_NegativeSign = result_negative;
}

void CBigInt::operator *= (int x) {
	CBigInt tmp (x);
	if (x == 0) {
		*this = tmp;
		return;
	}

	operator *= (tmp);

	if (tmp.isNegative() && (*this).isNegative())
		(*this).m_NegativeSign = 0;
	else if (tmp.isNegative() || (*this).isNegative())
		(*this).m_NegativeSign = 1;
}

void CBigInt::operator *= (const char * x) {
	CBigInt tmp (x);

	if (x[0] == '0') {
		*this = tmp;
		return;
	}

	operator *= (tmp);

	if (tmp.isNegative() && (*this).isNegative())
		(*this).m_NegativeSign = 0;
	else if (tmp.isNegative() || (*this).isNegative())
		(*this).m_NegativeSign = 1;
}

void CBigInt::operator *= (const CBigInt & a) {
	CBigInt min, max;
	vector<int> tmp;
	int m_X = 0;
	int m_Y = 0;
	int sum;
	int carry = 0;
	int size = -1;
	int i = 0;
	// int sum1;
	string s;

	// nastaveni minima a maxima
	if (*this > a) {
		max = *this;
		min = a;
	}
	else {
		max = a;
		min = *this;
	}

	// pruchod vectorem pro nasobeni
	// spodni cislo je prvni
	for (int j = 0; j < (int) min.m_Data.size(); j++) {

		// pro ulehceni prace
		m_Y = min.m_Data[j];

		// vrchni cislo je druhe
		i = 0;
		for ( ; i < (int) max.m_Data.size(); i++) {

			// pro ulehceni prace
			m_X = max.m_Data[i];

			sum = (m_X * m_Y) + carry;
			carry = sum / 10;
			sum %= 10;

			if (i + j > size) {
				tmp.push_back(sum);
				size = i + j;
			}
			else {
				tmp[i + j] += sum;
				carry += tmp[i + j] / 10;
				tmp[i + j] %= 10;
			}
		}

		if (carry > 0) {
			tmp.push_back(carry);
			size = i + j;
			carry = 0;
		}

	}

	// nahrazeni m_Data u puvodniho cisla
	m_Data = tmp;
	cout << tmp.max_size() << endl;
}

bool CBigInt::operator > (const CBigInt & a) {
	if (m_Data.size() == a.m_Data.size()) {
		for (int i = (int) m_Data.size(); i > 0; i--)
			if (m_Data[i - 1] > a.m_Data[i - 1])
				return true;

		return false;
	}
	else
		return (m_Data.size() > a.m_Data.size());
}

#ifndef __PROGTEST__
int main(int argc, const char * argv[]) {
	CBigInt       a, b;
	istringstream is;
	// a = 10;
	// a += 20;
	// cout << "r: 30; s: " << a;
	// a *= 5;
	// cout << "r: 150; s: " << a;
	// a *= 222;
	// cout << "r: 33300; s: " << a;
	// a = 150;
	// b = a + 3;
	// cout << "r: 153; s: " << b;
	// b = a * 7;
	// cout << "r: 1050; s: " << b;
	// cout << "r: 150; s: " << a;

	// a = 10;
	// a += -20;  // a = -10 
	// cout << "r: -10; s: " << a;
	// a *= 5;  // a = -50 
	// cout << "r: -50; s: " << a;
	// b = a + 73;  // b = 23
	// cout << "r: 23; s: " << b;
	// b = a * -7;  // b = 350
	// cout << "r: 350; s: " << b;
	// cout << "r: -50; s: " << a;  // -50

	// a = "12345678901234567890";
	// a += "-99999999999999999999";
	// cout << "r: -87654321098765432109" << endl << "s: " << a;
	// a *= "54321987654321987654";
	// cout << "r: -4761556948575111126880627366067073182286" << endl << "s: " << a;
	// a *= 0;  // a = 0
	// cout << "r: 0; s: " << a;
	// a = 10;
	// b = a + "400";  // b = 410
	// cout << "r: 410; s: " << b;
	// b = a * "15";  // b = 150 
	// cout << "r: 150; s: " << b;
	// cout << "r: 10; s: " << a;  // 10

	// is . clear ();
	// is . str ( " 1234" );
	// is >> b; // b = 1234, is . fail() = 0
	// cout << "r: 1234; s: " << b;
	// cout << "fail: 0 - " << is.fail() << endl;
	// is . clear ();
	// is . str ( " 12 34" );
	// is >> b; // b = 12, is . fail() = 0
	// cout << "r: 12; s: " << b;
	// cout << "fail: 0 - " << is.fail() << endl;
	// is . clear ();
	// is . str ( "999z" );
	// is >> b; // b = 999, is . fail() = 0
	// cout << "r: 999; s: " << b;
	// cout << "fail: 0 - " << is.fail() << endl;
	// is . clear ();
	// is . str ( "abcd" );
	// is >> b; // b = 999, is . fail() = 1
	// cout << "r: 999; s: " << b;
	// cout << "fail: 1 - " << is.fail() << endl;
	return 0;
}

#endif /*__PROGTEST__*/


// 628260075397164441344931005026506074151124920640099628107098937938189330457661955321677913255104461041916870345305101477546278342803884526246987398672326773262536368106725651613806984929887110121635274289542125317130568724531220850656485762441550819598466797855220993069113998353328186421755983384700280547634902446230578407326007738266437758698654860567675408024171936201887286260366451949343523137476827600159034979111227257228503212734947622813880691078701239959666241124234003729670402070966853398652918085566277761265213541062764637207818339917567990384874559183767466241890675570409919949712961010582351583466331997973727957298241165437202239184236118118057738242535612933524314858739610278542445317521823262888961803755360712186090661625431914107057268628456040605518952375833156866388683223747512167692464465797766865429949184752066554636479521757526875021484979556772452115080236215710150707079344453129171087776320760000431573913547918335603847921150020255473539635170948521642248996557642944723478192480416780550373286342316895469257453973630511513061428313352681850426222074216396077433593531337910497306295536850082887334774893075737528065852817057400008364101104372403074107220717294381023455745311468858498753055740559865917466739597215180364456609117196540592254731573938201588704635507172220135217552854634644636345350307029956696482040203177939766658049298027109922069558115272571078216555274887792991462513766288860918922330182436840230121583428590423781468787632341970541087332736872669244810822333151991332425770736928967236099332522450137211202622186751235696087753311106279999603185934655737331997958818224987378578405544803670022981404123829511140626063337440938014003764765214773018858298612653855872110782046825269647955414179005848600289711284044789279907531490484166674953571657408810493273775246351027119591621897471212925843315429565403510363399830478299642537671674183985412620191988901039929365987624385643994178856704500319124604716798567072223770207400376438453692699247244330203985076253206566402558394530495366333147359417108378282378635746498924143121378626166267789453216905879557554466107404482799660593020591712994109881958030234449507837203584013106875879452177029429066928590581440810558456949214879302708357586558607819252123784219016280000
// 064523299885085563259363422633848788232170199620688517983596110735982711369050241466811739808471123423296010037544983885739778276849925687349055762484966859294254323759953764220242107314761022522200845430375605724842891926919109116781561811780460048442043068206598297926374971568802174057891663122532736605039715023789337200575029624785291088905042581642728709361121119593699339888690200947354234171453686476436428778968061240750475600948356803099157626455852781675249816149510381890174093508110222396752152264184334931903489713979478161841911858509548208765633006724889113357510494216671920573530948489253792988310569276212