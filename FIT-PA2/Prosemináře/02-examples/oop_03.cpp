/* Skladani - odpovidajici si udaje o jednom automobilu jsou seskupena - do struktury TCar.
 * Odpada obrozvske mnozstvi parametru funkci. Stale ale musime predavat odkaz na seznam, se
 * kterym pracujeme.
 */

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
const int DBSIZE_MAX = 100;

struct TCar
 {
   string     m_RZ;
   int        m_Age;
   string     m_Owner;
 };



//-------------------------------------------------------------------------------------------------
bool               AddCar                                  ( TCar            * Cars,
                                                             int             & CarsNr,
                                                             const string    & RZ,
                                                             int               Age,
                                                             const string    & Owner )
 {
   int i;

   if ( CarsNr >= DBSIZE_MAX ) return ( false );

   for ( i = 0; i < CarsNr; i ++ )
    if ( Cars[i] . m_RZ == RZ )
     return ( false );

   Cars [CarsNr] . m_RZ    = RZ;
   Cars [CarsNr] . m_Age   = Age;
   Cars [CarsNr] . m_Owner = Owner;
   CarsNr  ++;
   return ( true );
 }
//-------------------------------------------------------------------------------------------------
bool               DelCar                                  ( TCar            * Cars,
                                                             int             & CarsNr,
                                                             const string    & RZ )
 {
   int i;

   for ( i = 0; i < CarsNr; i ++ )
    if ( Cars[i] . m_RZ == RZ )
     break;

   if ( i >= CarsNr ) return ( false );

   Cars[i] = Cars[--CarsNr];
   return ( true );
 }
//-------------------------------------------------------------------------------------------------
void               PrintAll                                ( TCar            * Cars,
                                                             int             & CarsNr )
 {
   int i;

   cout << "-------------------" << endl;
   for ( i = 0; i < CarsNr; i ++ )
    cout << Cars[i] . m_RZ << " " << Cars[i] . m_Age << " " << Cars[i] . m_Owner << endl;
 }
//-------------------------------------------------------------------------------------------------
void               SortByAge                               ( TCar            * Cars,
                                                             int             & CarsNr )
 {
   int       i, j, minIdx;
   TCar      tmp;

   for ( i = 0; i < CarsNr-1; i ++ )
    {
      minIdx = i;

      for ( j = i + 1; j < CarsNr; j ++ )
       if ( Cars[j] . m_Age < Cars[minIdx] . m_Age )
        minIdx = j;

      tmp = Cars[ minIdx ];
      Cars[minIdx] = Cars[i];
      Cars[i] = tmp;
    }
 }
//-------------------------------------------------------------------------------------------------
int main ( int argc, char * argv [] )
 {
   TCar        Cars1    [DBSIZE_MAX];
   int         CarsNr1  = 0;

   TCar        Cars2    [DBSIZE_MAX];
   int         CarsNr2  = 0;

   AddCar ( Cars2, CarsNr2, "3L8-45-67", 2, "Petr Novotny" );

   AddCar ( Cars1, CarsNr1, "ABC-12-24", 10, "Jan Novak" );
   AddCar ( Cars1, CarsNr1, "1AB-12-34",  4, "Jiri Svoboda" );
   AddCar ( Cars1, CarsNr1, "2K8-98-76",  3, "Jana Smutna" );


   PrintAll  ( Cars1, CarsNr1 );

   SortByAge ( Cars1, CarsNr1 );

   PrintAll  ( Cars1, CarsNr1 );

   DelCar    ( Cars1, CarsNr1, "1AB-12-34" );
   PrintAll  ( Cars1, CarsNr1 );


   PrintAll  ( Cars2, CarsNr2 );


   return ( 0 );
 }
