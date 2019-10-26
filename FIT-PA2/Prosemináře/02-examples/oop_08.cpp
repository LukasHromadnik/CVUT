/* Vylepseni - vyuziti c++ 11, STL
 */
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

class CCar
 {
   public:
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
                            ~CGarage                       ( void );

    bool                     AddCar                        ( const CCar      & X );

    bool                     DelCar                        ( const string    & RZ );

    void                     SortByAge                     ( void );
    void                     PrintAll                      ( void ) const;

   protected:
    vector<CCar>             m_List;
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
bool               CCar::EqualRZ                           ( const string    & RZ ) const
 {
   return m_RZ == RZ;
 }
//------------------------------------------------------------------------------------------------
bool               CCar::EqualRZ                           ( const CCar      & X ) const
 {
   return m_RZ == X . m_RZ;
 }
//------------------------------------------------------------------------------------------------
bool               CCar::NewerThan                         ( const CCar      & X ) const
 {
   return m_Year > X . m_Year;
 }
//------------------------------------------------------------------------------------------------
void               CCar::Print                             ( void ) const
 {
   time_t      t = time ( NULL );
   struct tm   Tm = *localtime ( &t );

   cout << m_RZ << " " << ( Tm . tm_year + 1900 - m_Year ) << " " << m_Owner;
 }
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
                   CGarage::CGarage                        ( void )
 {
 }
//------------------------------------------------------------------------------------------------
                   CGarage::~CGarage                       ( void )
 {
 }
//------------------------------------------------------------------------------------------------
bool               CGarage::AddCar                         ( const CCar      & x )
 {
   auto it = find_if ( m_List . begin (), m_List . end (), [&]( const CCar & y ) { return y . EqualRZ ( x ); } );

   if ( it != m_List . end () ) return false; // duplicate
   m_List . push_back ( x );
   return true;
 }
//-------------------------------------------------------------------------------------------------
bool               CGarage::DelCar                         ( const string    & RZ )
 {
   auto it = find_if ( m_List . begin (), m_List . end (), [&]( const CCar & y ) { return y . EqualRZ ( RZ ); } );

   if ( it == m_List . end () ) return false; // not found

   // amortized constant time:
  *it = m_List . back ();
   m_List . pop_back ();

   // linear time:
   // m_List . erase ( it );
   return true;
 }
//-------------------------------------------------------------------------------------------------
void               CGarage::PrintAll                       ( void ) const
 {
   cout << "-------------------" << endl;
   for ( const auto & car : m_List )
    {
      car . Print ();
      cout << endl;
    }
 }
//-------------------------------------------------------------------------------------------------
void               CGarage::SortByAge                      ( void )
 {
   sort ( m_List . begin (), m_List . end (), []( const CCar & a, const CCar & b ) { return a . NewerThan ( b ); } );
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
