/* Vylepseni - automobil i seznam jsou objekty. Kazdy objekt rozumi "sam sobe", vnejsimu svetu
 * poskytuje co rozhrani, nezatezuje vnejsi svet svoji vnitrni reprezentraci.
 *
 * V tomto reseni CGarage obsahuje pole staticky alokovanych objektu CCar, proto CCar musi
 * mit implicitni (bezparametricky) konstruktor.
 */

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class CCar
 {
   public:
                             CCar                          ( void );
                             CCar                          ( const string    & RZ,
                                                             int               Year,
                                                             const string    & Owner );


    bool                     EqualRZ                       ( const string    & RZ ) const;
    bool                     EqualRZ                       ( const CCar      & X ) const;
    bool                     NewerThan                     ( const CCar      & X ) const;
    void                     Print                         ( void ) const;


   protected:
    string                   m_RZ;
    int                      m_Year;
    string                   m_Owner;
 };


class CGarage
 {
   public:
                             CGarage                       ( void );

    bool                     AddCar                        ( const CCar      & X );

    bool                     DelCar                        ( const string    & RZ );

    void                     SortByAge                     ( void );
    void                     PrintAll                      ( void ) const;

   protected:
    static const int         DBSIZE_MAX = 100;

    CCar                     m_Cars [DBSIZE_MAX];
    int                      m_CarsNr;
 };

//------------------------------------------------------------------------------------------------
                   CCar::CCar                              ( const string    & RZ,
                                                             int               Year,
                                                             const string    & Owner )
 {
   m_RZ    = RZ;
   m_Year  = Year;
   m_Owner = Owner;
 }
//------------------------------------------------------------------------------------------------
                   CCar::CCar                              ( void )
 {
   m_RZ    = "";
   m_Year  = 0;
   m_Owner = "";
 }
//------------------------------------------------------------------------------------------------
bool               CCar::EqualRZ                           ( const string    & RZ ) const
 {
   return ( m_RZ == RZ );
 }
//------------------------------------------------------------------------------------------------
bool               CCar::EqualRZ                           ( const CCar      & X ) const
 {
   return ( m_RZ == X . m_RZ );
 }
//------------------------------------------------------------------------------------------------
bool               CCar::NewerThan                         ( const CCar      & X ) const
 {
   return ( m_Year > X . m_Year );
 }
//------------------------------------------------------------------------------------------------
void               CCar::Print                             ( void ) const
 {
   time_t      t;
   struct tm   Tm;

   t = time ( NULL );
   Tm = *localtime ( &t );

   cout << m_RZ << " " << ( Tm . tm_year + 1900 - m_Year ) << " " << m_Owner;
 }
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
                   CGarage::CGarage                        ( void )
 {
   m_CarsNr = 0;
 }
//------------------------------------------------------------------------------------------------
bool               CGarage::AddCar                         ( const CCar       & X )
 {
   int i;

   if ( m_CarsNr >= DBSIZE_MAX ) return ( false );

   for ( i = 0; i < m_CarsNr; i ++ )
    if ( m_Cars[i] . EqualRZ ( X ) )
     return ( false );

   m_Cars [m_CarsNr] = X;
   m_CarsNr  ++;
   return ( true );
 }
//-------------------------------------------------------------------------------------------------
bool               CGarage::DelCar                         ( const string    & RZ )
 {
   int i;

   for ( i = 0; i < m_CarsNr; i ++ )
    if ( m_Cars[i] . EqualRZ ( RZ ) )
     break;

   if ( i >= m_CarsNr ) return ( false );

   m_Cars[i] = m_Cars[--m_CarsNr];
   return ( true );
 }
//-------------------------------------------------------------------------------------------------
void               CGarage::PrintAll                       ( void ) const
 {
   int i;

   cout << "-------------------" << endl;
   for ( i = 0; i < m_CarsNr; i ++ )
    {
      m_Cars[i] . Print ( );
      cout << endl;
    }
 }
//-------------------------------------------------------------------------------------------------
void               CGarage::SortByAge                      ( void )
 {
   int       i, j, minIdx;
   CCar      tmp;

   for ( i = 0; i < m_CarsNr-1; i ++ )
    {
      minIdx = i;

      for ( j = i + 1; j < m_CarsNr; j ++ )
       if ( m_Cars[j] . NewerThan ( m_Cars[minIdx] ) )
        minIdx = j;

      tmp = m_Cars[ minIdx ];
      m_Cars[minIdx] = m_Cars[i];
      m_Cars[i] = tmp;
    }
 }
//-------------------------------------------------------------------------------------------------
int main ( int argc, char * argv [] )
 {
   CGarage      G1;
   CGarage      G2;


   G2 . AddCar ( CCar ( "3L8-45-67", 2008, "Petr Novotny" ) );

   G1 . AddCar ( CCar ( "ABC-12-24", 2000, "Jan Novak" ) );
   G1 . AddCar ( CCar ( "1AB-12-34", 2006, "Jiri Svoboda" ) );
   G1 . AddCar ( CCar ( "2K8-98-76", 2007, "Jana Smutna" ) );


   G1 . PrintAll  ( );

   G1 . SortByAge ( );

   G1 . PrintAll  ( );

   G1 . DelCar    ( "1AB-12-34" );
   G1 . PrintAll  ( );


   G2 . PrintAll  ( );

   return ( 0 );
 }
