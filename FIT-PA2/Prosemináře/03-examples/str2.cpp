#include <cstring>
#include <iostream>
using namespace std;



class CStrAuto
 {
   public:
                             CStrAuto            ( const char * Str = "" );
    explicit                 CStrAuto            ( double X );
    explicit                 CStrAuto            ( int X );
                             CStrAuto            ( const CStrAuto & x );
                            ~CStrAuto            ( void );
    CStrAuto               & operator =          ( const CStrAuto & x );
    int                      Length              ( void ) const;
    CStrAuto               & operator +=         ( const CStrAuto & x );
    CStrAuto               & operator +=         ( const char * x );
    CStrAuto               & operator +=         ( int x );
    CStrAuto               & operator +=         ( double x );
                             operator const char * ( void ) const;
    char                     operator []         ( int idx ) const;
    void                     SetCharAt           ( int idx, char X );

   protected:
    struct TData
     {
                             TData               ( const char * str );
                             TData               ( int Reserve );
                            ~TData               ( void );
       int                   RefCnt;
       int                   Len;
       int                   Max;
       char                * Data;
     };

    TData                  * m_Data;


    void                     append              ( const char * src, int srclen );
    void                     detach              ( int newMaxSize );
                                                 
    friend CStrAuto          operator +          ( const CStrAuto & a, const CStrAuto & b );
    friend std::ostream    & operator <<         ( std::ostream & os, const CStrAuto & x );
 };

CStrAuto           operator +                    ( const CStrAuto & a, const CStrAuto & b );
std::ostream     & operator <<                   ( std::ostream & os, const CStrAuto & x );




//-------------------------------------------------------------------------------------------------
                   CStrAuto::TData::TData        ( const char * Str )
 {
   RefCnt    = 1;
   Len       = strlen ( Str );
   Max       = Len + 1;
   Data      = new char[Max];
   memcpy    ( Data, Str, Len + 1 );
 }                   
//-------------------------------------------------------------------------------------------------
                   CStrAuto::TData::TData        ( int Reserve )
 {
   RefCnt    = 1;
   Len       = 0;
   Max       = Reserve;
   Data      = new char[Max];
   Data[0]   = 0;
 }                   
//-------------------------------------------------------------------------------------------------
                   CStrAuto::TData::~TData       ( void )
 {
   delete [] Data;
 }
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
                   CStrAuto::CStrAuto            ( const char * Str )
 {
   m_Data           = new TData ( Str );
 }
//-------------------------------------------------------------------------------------------------
                   CStrAuto::CStrAuto            ( double X )
 {
   m_Data           = new TData ( 20 );
   operator += ( X );
 }
//-------------------------------------------------------------------------------------------------
                   CStrAuto::CStrAuto            ( int X )
 {
   m_Data           = new TData ( 20 );
   operator += ( X );
 }
//-------------------------------------------------------------------------------------------------
                   CStrAuto::CStrAuto            ( const CStrAuto & x )
 {
   m_Data = x . m_Data;
   m_Data -> RefCnt ++;
 }
//-------------------------------------------------------------------------------------------------
                   CStrAuto::~CStrAuto           ( void )
 {
   if ( -- m_Data -> RefCnt == 0 ) delete m_Data;
 }
//-------------------------------------------------------------------------------------------------
CStrAuto         & CStrAuto::operator =          ( const CStrAuto & x )
 {
   if ( &x != this )
    {
      if ( -- m_Data -> RefCnt == 0 )
       {
         delete m_Data;
       }
      m_Data = x . m_Data;
      m_Data -> RefCnt ++;
    }
   return ( *this );
 }
//-------------------------------------------------------------------------------------------------
int                CStrAuto::Length              ( void ) const
 {
   return ( m_Data -> Len );
 }
//-------------------------------------------------------------------------------------------------
CStrAuto         & CStrAuto::operator +=         ( const CStrAuto & x )
 {
   append ( x . m_Data -> Data, x . m_Data -> Len );
   return ( * this );
 }
//-------------------------------------------------------------------------------------------------
CStrAuto         & CStrAuto::operator +=         ( const char * x )
 {
   append ( x, strlen ( x ) );
   return ( * this );
 }
//-------------------------------------------------------------------------------------------------
CStrAuto         & CStrAuto::operator +=         ( int x )
 {
   char tmp [20];

   snprintf ( tmp, sizeof ( tmp ), "%d", x );
   append ( tmp, strlen ( tmp ) );
   return ( *this );
 }
//-------------------------------------------------------------------------------------------------
CStrAuto         & CStrAuto::operator +=         ( double x )
 {
   char tmp [20];
 
   snprintf ( tmp, sizeof ( tmp ), "%f", x );
   append ( tmp, strlen ( tmp ) );
   return ( *this );
 }
//-------------------------------------------------------------------------------------------------
                   CStrAuto::operator const char * ( void ) const
 {
   return ( m_Data -> Data );
 }
//-------------------------------------------------------------------------------------------------
char               CStrAuto::operator []         ( int idx ) const
 {
   if ( idx < 0 || idx >= m_Data -> Len )
    throw "index out of range";

   return ( m_Data -> Data[idx] );
 }
//-------------------------------------------------------------------------------------------------
void               CStrAuto::SetCharAt           ( int idx, char X )
 {
   if ( idx < 0 || idx >= m_Data -> Len )
    throw "index out of range";

   detach ( m_Data -> Max );
   m_Data -> Data[idx] = X;
 }
//-------------------------------------------------------------------------------------------------
void               CStrAuto::append              ( const char * src, int srclen )
 {
   char  * tmp;

   detach ( m_Data -> Len + srclen + 1 );

   if ( m_Data -> Len + srclen + 1 > m_Data -> Max )
     {
       m_Data -> Max = m_Data -> Len + srclen + 1;
       tmp = new char [m_Data -> Max];
       memcpy ( tmp, m_Data -> Data, m_Data -> Len );
       memcpy ( tmp + m_Data -> Len, src, srclen );
       delete [] m_Data -> Data;  // safe even if m_Data == src
       m_Data -> Data = tmp;
     }
    else 
     memcpy ( m_Data -> Data + m_Data -> Len, src, srclen );
   m_Data -> Len += srclen;
   m_Data -> Data[m_Data -> Len] = 0;
 }
//-------------------------------------------------------------------------------------------------
void               CStrAuto::detach              ( int newMaxSize )
 {
   TData * tmpD;

   if ( m_Data -> RefCnt > 1 )
    { // data block is shared among more than one instance of CStrAuto. create own block
      // that can be modified without affecting the other CStrAuto instances.
      
      tmpD = new TData ( newMaxSize );
      memcpy ( tmpD -> Data, m_Data -> Data, m_Data -> Len + 1 ); // + terminating zero
         // tmpD is a separate data block for use in our instance. disconnect from the source
   
      m_Data -> RefCnt --;
      m_Data = tmpD;
    }
 }
//-------------------------------------------------------------------------------------------------
CStrAuto            operator +                   ( const CStrAuto & a, const CStrAuto & b )
 {
   CStrAuto res ( a );
   res += b;
   return ( res );
 }
//-------------------------------------------------------------------------------------------------
std::ostream     & operator <<                   ( std::ostream & os, const CStrAuto & x )
 {
   os << x . m_Data -> Data;
   return ( os );
 }
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int                main                          ( int argc, char * argv [] )
 {
   CStrAuto      a ( 5 ), b = "Pokusny retezec";
   int       i;
   
   a += " test ";
   
   CStrAuto      c ( a );                       // copy konstruktor
   
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
   
   b = a + CStrAuto ( 5 ); // ok
   b = a + CStrAuto ( 5.0 ); // ok

   return ( 0 );
 }
//-------------------------------------------------------------------------------------------------
