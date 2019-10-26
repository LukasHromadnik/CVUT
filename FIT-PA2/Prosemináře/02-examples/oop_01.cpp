/* Zakladni reseni - pouze jeden seznam vozidel, v globalnich promennych,
 * odpovidajici si data jsou na stejnem indexu v polich.
 *
 * Problem - co kdyz je potreba vice takovych ruznych seznamu.
 */

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
const int DBSIZE_MAX = 100;

string      g_RZ      [DBSIZE_MAX];
int         g_Age     [DBSIZE_MAX];
string      g_Owner   [DBSIZE_MAX];
int         g_Cars    = 0;


//-------------------------------------------------------------------------------------------------
bool               AddCar                                  ( const string    & RZ,
                                                             int               Age,
                                                             const string    & Owner )
 {
   int i;

   if ( g_Cars >= DBSIZE_MAX ) return ( false );

   for ( i = 0; i < g_Cars; i ++ )
    if ( g_RZ[i] == RZ )
     return ( false );

   g_RZ [g_Cars]     = RZ;
   g_Age [g_Cars]    = Age;
   g_Owner[g_Cars]   = Owner;
   g_Cars ++;
   return ( true );
 }
//-------------------------------------------------------------------------------------------------
bool               DelCar                                  ( const string    & RZ )
 {
   int i;

   for ( i = 0; i < g_Cars; i ++ )
    if ( g_RZ[i] == RZ )
     break;

   if ( i >= g_Cars ) return ( false );

   g_Cars --;
   g_RZ[i]      = g_RZ[g_Cars];
   g_Age[i]     = g_Age [g_Cars];
   g_Owner[i]   = g_Owner [g_Cars];

   return ( true );
 }
//-------------------------------------------------------------------------------------------------
void               PrintAll                                ( void )
 {
   int i;

   cout << "-------------------" << endl;
   for ( i = 0; i < g_Cars; i ++ )
    cout << g_RZ[i] << " " << g_Age[i] << " " << g_Owner[i] << endl;
 }
//-------------------------------------------------------------------------------------------------
void               SortByAge                               ( void )
 {
   int       i, j, tmpInt, minIdx;
   string    tmpStr;

   for ( i = 0; i < g_Cars-1; i ++ )
    {
      minIdx = i;

      for ( j = i + 1; j < g_Cars; j ++ )
       if ( g_Age[j] < g_Age[minIdx] )
        minIdx = j;

      tmpStr = g_RZ[minIdx] ; g_RZ[minIdx] = g_RZ[i]; g_RZ[i] = tmpStr;
      tmpInt = g_Age[minIdx]; g_Age[minIdx] = g_Age[i]; g_Age[i] = tmpInt;
      tmpStr = g_Owner[minIdx]; g_Owner[minIdx] = g_Owner[i]; g_Owner[i] = tmpStr;
    }
 }
//-------------------------------------------------------------------------------------------------
int main ( int argc, char * argv [] )
 {
   AddCar ( "ABC-12-24", 10, "Jan Novak" );
   AddCar ( "1AB-12-34",  4, "Jiri Svoboda" );
   AddCar ( "2K8-98-76",  3, "Jana Smutna" );


   PrintAll ();

   SortByAge ();

   PrintAll ();

   DelCar ( "1AB-12-34" );
   PrintAll ();
   return ( 0 );
 }

