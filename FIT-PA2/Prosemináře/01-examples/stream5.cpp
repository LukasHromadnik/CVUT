#include <climits>  // INT_MAX
#include <cmath>    // sqrt, atan2
#include <iostream>
#include <iomanip>
#include <sstream>  // istringstream

using namespace std;


bool parseCoord ( string s, double & x, double & y )
 {
   istringstream is ( s );
   char dummy1, dummy2, dummy3;
   
   is >> dummy1 >> x >> dummy2 >> y >> dummy3;
   if ( is . fail () || dummy1 != '(' || dummy2 != ',' || dummy3 != ')' ) return ( false );

   // zatim neni eof - cteni dummy3 se zdarilo, nebyl pokus o cteni _za_koncem_ souboru

   is . get (); // zkusime nacist 1 dalsi znak (libovolny, i bily znak). Tim musime dosahnout za EOF.

   return ( is . eof () );
 }

int main ( int argc, char * argv [] )
 {
   double  x, y;
   string  line;
    
   cout << "Zadejte souradnice ve formatu (x,y):" << endl;
   
   while ( getline ( cin, line ) )
    { 
      if ( ! parseCoord ( line, x, y ) ) break;
      cout << '(' << x << ',' << y << ") = (" << 
           sqrt ( x * x + y * y ) << '<' << atan2 ( y, x ) << ')' << endl;
    }
   if ( ! cin . eof () )
    {
      cout << "Nespravny vstup." << endl;
      return ( 1 );
    }
   return ( 0 ); 
 }

