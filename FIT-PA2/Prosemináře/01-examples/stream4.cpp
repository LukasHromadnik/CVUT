#include <climits>  // INT_MAX
#include <iostream>
#include <iomanip>
using namespace std;


int main ( int argc, char * argv [] )
 {
   int a, b;
    
   cout << "Zadejte dve kladna cisla:" << endl;
   
   cin >> a >> b;
   
   while ( cin . fail () || a <= 0 || b <= 0 )
    { 
      // chyba zpusobi, ze stream nereaguje na dalsi operace, dokud priznak nevynulujeme
      cin . clear ();
      // odstranit to, co zbylo na vstupu, do konce radky
      cin . ignore ( INT_MAX, '\n' );
      cout << "Chybne zadani. Zadejte prosim dve kladna cisla:" << endl;
      cin >> a >> b;
    }

   cout << "Zadano: " << a << ", " << b << endl;

   return ( 0 ); 
 }

