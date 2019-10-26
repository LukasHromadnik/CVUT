#include <iostream>
#include <iomanip>
using namespace std;


int main ( int argc, char * argv [] )
 {
   int x, sum = 0;
    
   cout << "Zadejte posloupnost nezapornych cisel, konec eof:" << endl;
   
   while ( 1 )
    {
      cin >> x;
      if ( cin . fail () ) break;
      if ( x < 0 ) break;
      sum += x;
    }
   // nepodarilo se nacist vstup -> bud nespravny vstup, nebo jsme na konci (EOF)
   if ( ! cin . eof () )
    {
      cout << "Nespravny vstup." << endl;
      return ( 1 );
    }

   cout << "Soucet = " << sum << endl;
   return ( 0 ); 
 }

