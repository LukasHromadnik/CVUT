#include <cstdlib> // abs ()
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;


// Hlaseni chyby - je lepsi hazet vyjimky jako popisne tridy.
class InvalidOperandException { };

class CRat
 {
   public:
                             CRat                ( int num = 0, int denom = 1 );
                            ~CRat                ( void )  { }    // zde neni potreba
    CRat                     operator -          ( void ) const;  // unarni minus
                             operator  double    ( void ) const;  // operator pretypovani na double

    CRat                     operator +          ( int x ) const;
    CRat                     operator -          ( int x ) const;
    CRat                     operator +          ( const CRat & b ) const;
    CRat                     operator -          ( const CRat & b ) const;
    friend CRat              operator +          ( int a, const CRat & b );
    friend CRat              operator -          ( int a, const CRat & b );

    friend ostream         & operator <<         ( ostream & os, const CRat & x );
    friend istream         & operator >>         ( istream & is, CRat & x );

   protected:
    void                     simplify            ( void );

    int                      m_Num;
    int                      m_Denom;
 };

//-------------------------------------------------------------------------------------------------
                   CRat::CRat                    ( int num, int denom )
 {
   if ( denom == 0 ) throw InvalidOperandException();
   m_Num   = num;
   m_Denom = denom;
   simplify ();
 }
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
CRat               CRat::operator -              ( void ) const
 {
   // operator unarni - v konvencnim pojeti nemeni hodnotu operandu. Vraci novou hodnotu, s opacnym znamenkem.
   return ( CRat ( - m_Num, m_Denom ) );
 }
//-------------------------------------------------------------------------------------------------
                   CRat::operator  double        ( void ) const
 {
   // pretypovani na double
   return ( (double) m_Num / m_Denom );
 }
//-------------------------------------------------------------------------------------------------
void               CRat::simplify                ( void )
 {
   int a, b, tmp;

   // zkraceni zlomku - pomoc nejvetsiho spol. delitele.
   a = abs ( m_Num );
   b = abs ( m_Denom );

   if ( !a || !b ) return;

   if ( a < b )
    {
      tmp = a;
      a = b;
      b = tmp;
    }

   while ( b )
    {
      tmp = a % b;
      a = b;
      b = tmp;
    }

   m_Num   /= a;
   m_Denom /= a;
 }
//-------------------------------------------------------------------------------------------------
CRat               CRat::operator +              ( const CRat & b ) const
 {
   return ( CRat ( m_Num * b . m_Denom + b . m_Num * m_Denom, m_Denom * b . m_Denom ) );
 }
//-------------------------------------------------------------------------------------------------
CRat               CRat::operator -              ( const CRat & b ) const
 {
   return ( CRat ( m_Num * b . m_Denom - b . m_Num * m_Denom, m_Denom * b . m_Denom ) );
 }
//-------------------------------------------------------------------------------------------------
CRat               CRat::operator +              ( int b ) const
 {
   return ( CRat ( m_Num + b * m_Denom, m_Denom ) );
 }
//-------------------------------------------------------------------------------------------------
CRat               CRat::operator -              ( int b ) const
 {
   return ( CRat ( m_Num - b * m_Denom, m_Denom ) );
 }
//-------------------------------------------------------------------------------------------------
ostream          & operator <<                   ( ostream & os, const CRat & x )
 {
   os << x . m_Num << "/" << x . m_Denom;
   return ( os );
 }
//-------------------------------------------------------------------------------------------------
istream          & operator >>                   ( istream & is, CRat & x )
 {
   char          c;
   int           n, d;

   is >> n >> c >> d; // instanci x nemenime dokud nenacteme a nezkontrolujeme
                      // vsechna vstupni data

   if ( is . fail () || d == 0 || c != '/' )
    {
      // timto nastavujeme fail bit
      is . setstate ( ios::failbit );
      return ( is );
    }
   x . m_Num   = n;
   x . m_Denom = d;
   x . simplify ();
   return ( is );
 }
//-------------------------------------------------------------------------------------------------
CRat               operator +                    ( int a, const CRat & b )
 {
   return b + a;
 }
//-------------------------------------------------------------------------------------------------
CRat               operator -                    ( int a, const CRat & b )
 {
   return -b + a;
 }
//-------------------------------------------------------------------------------------------------
int                main                          ( int argc, char * argv [] )
 {
   CRat a ( 3, 8 ), b ( 9, 8 ), c;
   istringstream is1 ( "2 / 8" ), is2 ( "6.7 / 3" );
   double z;


   is1 >> c;
   cout << "is1 read failure: " << is1 . fail () << " c = " << c << endl;
   is2 >> c;
   cout << "is2 read failure: " << is2 . fail () << " c = " << c << endl;

   cout << a << " + " << b << " = " << (a + b) << endl;
   cout << c << endl;

   c = c - 3; // what about c = c * 3

   z = c;

   cout << c << " = " << z << endl;


   return ( 0 );
 }
//-------------------------------------------------------------------------------------------------
