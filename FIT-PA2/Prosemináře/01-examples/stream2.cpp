#include <iostream>
#include <iomanip>
using namespace std;


int main ( int argc, char * argv [] )
 {
   int    a = 10;
   double b = 20;
   const char * c = "test";
   
   
   cout << "a = " << setw ( 50 ) << a << endl;

   cout << "a = " << setw ( 50 ) << setfill ( '*' ) << a << endl;
   
   cout << "b = " << setw ( 20 ) << setfill ( ' ' ) << setprecision ( 3 ) << b << endl;
   cout << fixed;
   cout << "b = " << setw ( 20 ) << setfill ( ' ' ) << setprecision ( 3 ) << b << endl;
      
   cout << "c = " << c << endl;

   cout << "a = " << hex << a << " (hex) = " << oct << a << " (oct)"  << endl; 
   cout << dec; // vratime zpet dec

   cout << "c = " << setw ( 50 ) << left << c << endl;
   cout << "c = " << setw ( 50 ) << right << c << endl;


   return ( 0 ); 
 }

