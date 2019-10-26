#include <cstring>
#include <iostream>
using namespace std;


// trida implementujici natahovaci retezec + zakladni operace s nim

class CStr
 {
   public:
                             CStr                ( const char * Str = "" );
    explicit                 CStr                ( double X );
    explicit                 CStr                ( int X );
                             CStr                ( const CStr & x );
                            ~CStr                ( void );
    CStr                   & operator =          ( const CStr & x );
    int                      Length              ( void ) const;
    CStr                   & operator +=         ( const CStr & x );
    CStr                   & operator +=         ( const char * x );
    CStr                   & operator +=         ( int x );
    CStr                   & operator +=         ( double x );
                             operator const char * ( void ) const;
    char                   & operator []         ( int idx );

   protected:
    int                      m_Len;
    int                      m_Max;
    char                   * m_Data;

    void                     append              ( const char * src, int srclen );

    friend CStr              operator +          ( const CStr & a, const CStr & b );
    friend std::ostream    & operator <<         ( std::ostream & os, const CStr & x );
 };

CStr               operator +                    ( const CStr & a, const CStr & b );
std::ostream     & operator <<                   ( std::ostream & os, const CStr & x );



//-------------------------------------------------------------------------------------------------
                   CStr::CStr                    ( const char * Str )
 {
   m_Len   = strlen ( Str );
   m_Max   = m_Len + 1;
   m_Data  = new char[m_Max];
   strncpy ( m_Data, Str, m_Max );
 }
//-------------------------------------------------------------------------------------------------
                   CStr::CStr                    ( double X )
 {
   m_Max     = 20;
   m_Data    = new char [m_Max];
   m_Len     = 0;
  *m_Data    = 0;
   operator += ( X );
 }
//-------------------------------------------------------------------------------------------------
                   CStr::CStr                    ( int X )
 {
   m_Max     = 20;
   m_Data    = new char [m_Max];
   m_Len     = 0;
  *m_Data    = 0;
   operator += ( X );
 }
//-------------------------------------------------------------------------------------------------
                   CStr::CStr                    ( const CStr & x )
 {
   m_Max     = x . m_Max;
   m_Len     = x . m_Len;
   m_Data    = new char [m_Max];
   memcpy    ( m_Data, x . m_Data, m_Len + 1 );
 }
//-------------------------------------------------------------------------------------------------
                   CStr::~CStr                   ( void )
 {
   delete [] m_Data;
 }
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator =              ( const CStr & x )
 {
   if ( &x != this )
    {
      delete [] m_Data;
      m_Max   = x . m_Max;
      m_Len   = x . m_Len;
      m_Data  = new char [m_Max];
      memcpy  ( m_Data, x . m_Data, m_Len + 1 );
    }
   return ( *this );
 }
//-------------------------------------------------------------------------------------------------
int                CStr::Length                  ( void ) const
 {
   return ( m_Len );
 }
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator +=             ( const CStr & x )
 {
   append ( x . m_Data, x . m_Len );
   return ( * this );
 }
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator +=             ( const char * x )
 {
   append ( x, strlen ( x ) );
   return ( * this );
 }
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator +=             ( int x )
 {
   char tmp [20];
   
   // nepouzivame ostringstream.
   snprintf  ( tmp, sizeof ( tmp ), "%d", x );
   append    ( tmp, strlen ( tmp ) );
   return    ( *this );
 }
//-------------------------------------------------------------------------------------------------
CStr             & CStr::operator +=             ( double x )
 {
   char tmp [20];
   
   snprintf ( tmp, sizeof ( tmp ), "%f", x );
   append ( tmp, strlen ( tmp ) );
   return ( *this );
 }
//-------------------------------------------------------------------------------------------------
                   CStr::operator const char *   ( void ) const
 {
   return ( m_Data );
 }
//-------------------------------------------------------------------------------------------------
char             & CStr::operator []             ( int idx )
 {
   if ( idx < 0 || idx >= m_Len )
    throw "index out of range";

   return ( m_Data[idx] );
 }
//-------------------------------------------------------------------------------------------------
void              CStr::append                   ( const char * src, int srclen )
 {
   char * tmp;
   
   if ( m_Len + srclen + 1 >= m_Max )
    {
      m_Max = m_Len + srclen + 1;
      tmp = new char [m_Max];
      memcpy ( tmp, m_Data, m_Len );
      memcpy ( tmp + m_Len, src, srclen ); 
      delete [] m_Data; // safe even if m_Data == src
      m_Data = tmp;
    }
   else 
    memcpy         ( m_Data + m_Len, src, srclen );
   m_Len         += srclen;
   m_Data[m_Len]  = 0;
 }
//-------------------------------------------------------------------------------------------------
CStr              operator +                     ( const CStr & a, const CStr & b )
 {
   CStr res ( a );
   res += b;
   return ( res );
 }
//-------------------------------------------------------------------------------------------------
std::ostream    & operator <<                    ( std::ostream & os, const CStr & x )
 {
   os << x . m_Data;
   return ( os );
 }
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int               main                           ( int argc, char * argv [] )
 {
   CStr      a ( 5 ), b = "Pokusny retezec";
   int       i;
   
   a += " test ";
   
   CStr      c ( a );                       // copy konstruktor
   
   a += 12.5;                               // testy prettizeni op +=
   a += b;
   
   cout << "a = " << a << endl <<
           "b = " << b << endl <<
           "c = " << c << endl;
   
   for ( i = a . Length  () - 1; i >= 0; i -- )  // test op []
    cout << a[i];
   cout << endl;
   
   c = b;
   
   const char * pokus = (const char*)  c;       // test op pretypovani
   
   cout << pokus << endl;
   
   c = a + b;                                // test op. zretezeni
   
   cout << "a = " << a << endl <<
           "b = " << b << endl <<
           "c = " << c << endl;

   c = a + "pokus";                               // test kons. uziv konverze
   
   // b = a + 5;   // kdo uhodne co se stane?
   // b = a + 5.0; // nejde, konstruktor je explicitni
   
   b = a + CStr ( 5 ); // ok
   b = a + CStr ( 5.0 ); // ok


   cout << "a = " << a << endl <<
           "b = " << b << endl <<
           "c = " << c << endl;

   return ( 0 );
 }
//-------------------------------------------------------------------------------------------------
