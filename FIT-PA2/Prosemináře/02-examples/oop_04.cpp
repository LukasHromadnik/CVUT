/* Vylepseni - seznam automobilu je reprezentovan objektem, manipulace s daty probiha jeho metodami.
 * Parametry neni potreba predavat explicitne, metody je maji k dispozici implicitne.
 *
 * Problem - co kdyz se zmeni vnitrni reprezentace automobilu? Napr. nebudeme ukladat stari, ale rok
 * vyroby vozu.
 */
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

struct TCar
 {
   string     m_RZ;
   int        m_Age;
   string     m_Owner;
 };


class CGarage
 {
   public:
                             CGarage                       ( void );

    bool                     AddCar                        ( const string    & RZ,
                                                             int               Age,
                                                             const string    & Owner );

    bool                     DelCar                        ( const string    & RZ );

    void                     SortByAge                     ( void );
    void                     PrintAll                      ( void ) const;

   protected:
    static const int         DBSIZE_MAX = 100;

    TCar                     m_Cars [DBSIZE_MAX];
    int                      m_CarsNr;
 };

//------------------------------------------------------------------------------------------------
                   CGarage::CGarage                        ( void )
 {
   m_CarsNr = 0;
 }
//------------------------------------------------------------------------------------------------
bool               CGarage::AddCar                         ( const string    & RZ,
                                                             int               Age,
                                                             const string    & Owner )
 {
   int i;

   if ( m_CarsNr >= DBSIZE_MAX ) return ( false );

   for ( i = 0; i < m_CarsNr; i ++ )
    if ( m_Cars[i] . m_RZ == RZ )
     return ( false );

   m_Cars [m_CarsNr] . m_RZ    = RZ;
   m_Cars [m_CarsNr] . m_Age   = Age;
   m_Cars [m_CarsNr] . m_Owner = Owner;
   m_CarsNr  ++;
   return ( true );
 }
//-------------------------------------------------------------------------------------------------
bool               CGarage::DelCar                         ( const string    & RZ )
 {
   int i;

   for ( i = 0; i < m_CarsNr; i ++ )
    if ( m_Cars[i] . m_RZ == RZ )
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
    cout << m_Cars[i] . m_RZ << " " << m_Cars[i] . m_Age << " " << m_Cars[i] . m_Owner << endl;
 }
//-------------------------------------------------------------------------------------------------
void               CGarage::SortByAge                      ( void )
 {
   int       i, j, minIdx;
   TCar      tmp;

   for ( i = 0; i < m_CarsNr-1; i ++ )
    {
      minIdx = i;

      for ( j = i + 1; j < m_CarsNr; j ++ )
       if ( m_Cars[j] . m_Age < m_Cars[minIdx] . m_Age )
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


   G2 . AddCar ( "3L8-45-67", 2, "Petr Novotny" );

   G1 . AddCar ( "ABC-12-24", 10, "Jan Novak" );
   G1 . AddCar ( "1AB-12-34",  4, "Jiri Svoboda" );
   G1 . AddCar ( "2K8-98-76",  3, "Jana Smutna" );


   G1 . PrintAll  ( );

   G1 . SortByAge ( );

   G1 . PrintAll  ( );

   G1 . DelCar    ( "1AB-12-34" );
   G1 . PrintAll  ( );


   G2 . PrintAll  ( );

   return ( 0 );
 }
