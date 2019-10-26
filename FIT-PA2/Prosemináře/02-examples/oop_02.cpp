/* Oprava - lze pracovat s vice nezavislymi seznamy. Odkazy na odpovidajici pole jsou
 * predavane v paramerech. Extremne neprakticke, zejmena pokud by uchovavanych atributu
 * bylo vice. Pridani atributu = prepis celeho zbytku programu
 */

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
const int DBSIZE_MAX = 100;



//-------------------------------------------------------------------------------------------------
bool               AddCar                                  ( string          * l_RZ,
                                                             int             * l_Age,
                                                             string          * l_Owner,
                                                             int             & l_Cars,
                                                             const string    & RZ,
                                                             int               Age,
                                                             const string    & Owner )
 {
   int i;

   if ( l_Cars >= DBSIZE_MAX ) return ( false );

   for ( i = 0; i < l_Cars; i ++ )
    if ( l_RZ[i] == RZ )
     return ( false );

   l_RZ [l_Cars]     = RZ;
   l_Age [l_Cars]    = Age;
   l_Owner[l_Cars]   = Owner;
   l_Cars ++;
   return ( true );
 }
//-------------------------------------------------------------------------------------------------
bool               DelCar                                  ( string          * l_RZ,
                                                             int             * l_Age,
                                                             string          * l_Owner,
                                                             int             & l_Cars,
                                                             const string    & RZ )
 {
   int i;

   for ( i = 0; i < l_Cars; i ++ )
    if ( l_RZ[i] == RZ )
     break;

   if ( i >= l_Cars ) return ( false );

   l_Cars --;
   l_RZ[i]      = l_RZ[l_Cars];
   l_Age[i]     = l_Age [l_Cars];
   l_Owner[i]   = l_Owner [l_Cars];

   return ( true );
 }
//-------------------------------------------------------------------------------------------------
void               PrintAll                                ( string          * l_RZ,
                                                             int             * l_Age,
                                                             string          * l_Owner,
                                                             int               l_Cars )
 {
   int i;

   cout << "-------------------" << endl;
   for ( i = 0; i < l_Cars; i ++ )
    cout << l_RZ[i] << " " << l_Age[i] << " " << l_Owner[i] << endl;
 }
//-------------------------------------------------------------------------------------------------
void               SortByAge                               ( string          * l_RZ,
                                                             int             * l_Age,
                                                             string          * l_Owner,
                                                             int               l_Cars )
 {
   int       i, j, tmpInt, minIdx;
   string    tmpStr;

   for ( i = 0; i < l_Cars-1; i ++ )
    {
      minIdx = i;

      for ( j = i + 1; j < l_Cars; j ++ )
       if ( l_Age[j] < l_Age[minIdx] )
        minIdx = j;

      tmpStr = l_RZ[minIdx] ; l_RZ[minIdx] = l_RZ[i]; l_RZ[i] = tmpStr;
      tmpInt = l_Age[minIdx]; l_Age[minIdx] = l_Age[i]; l_Age[i] = tmpInt;
      tmpStr = l_Owner[minIdx]; l_Owner[minIdx] = l_Owner[i]; l_Owner[i] = tmpStr;
    }
 }
//-------------------------------------------------------------------------------------------------
int main ( int argc, char * argv [] )
 {
   string      RZ1      [DBSIZE_MAX];
   int         Age1     [DBSIZE_MAX];
   string      Owner1   [DBSIZE_MAX];
   int         Cars1    = 0;

   string      RZ2      [DBSIZE_MAX];
   int         Age2     [DBSIZE_MAX];
   string      Owner2   [DBSIZE_MAX];
   int         Cars2    = 0;

   AddCar ( RZ2, Age2, Owner2, Cars2, "3L8-45-67", 2, "Petr Novotny" );

   AddCar ( RZ1, Age1, Owner1, Cars1, "ABC-12-24", 10, "Jan Novak" );
   AddCar ( RZ1, Age1, Owner1, Cars1, "1AB-12-34",  4, "Jiri Svoboda" );
   AddCar ( RZ1, Age1, Owner1, Cars1, "2K8-98-76",  3, "Jana Smutna" );


   PrintAll  ( RZ1, Age1, Owner1, Cars1 );

   SortByAge ( RZ1, Age1, Owner1, Cars1 );

   PrintAll  ( RZ1, Age1, Owner1, Cars1 );

   DelCar    ( RZ1, Age1, Owner1, Cars1, "1AB-12-34" );
   PrintAll  ( RZ1, Age1, Owner1, Cars1 );


   PrintAll  ( RZ2, Age2, Owner2, Cars2 );


   return ( 0 );
 }
