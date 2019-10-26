#include <iostream>
#include <iomanip>
#include <sstream>  // ostringstream
#include <fstream>  // fstream

using namespace std;


bool dumpFile ( const char * FileName )
 { 
   ifstream      f ( FileName, ios::binary | ios::in );
   ostringstream ob;
   int           offset = 0;
   char          c;
   
   // soubor neslo otevrit -> chyba
   if ( f . fail () ) return ( false );
   
   while ( f . get ( c ) )
    {
      // 16 bajtu na radku, tedy kazdy 16-ty bakt vypsat adresu
      if ( offset % 16 == 0 )
       cout << hex << setw ( 8 ) << setfill ( '0' ) << offset;

      // bajt (char) prevedeme na neznamenkovou hodnotu
      cout << ' ' << hex << setw ( 2 ) << (int)(unsigned char)c;
      if ( ! isprint ( c ) )
        ob << '?'; // pokud neni tisknutelny, nahradit ?
       else
        ob << c;
        
      // za 16-tym znakem na radce zobrazit odradkovani (textovou reprezentaci)
      if ( offset % 16 == 15 )
       {
         cout << ' ' << ob . str  () << endl;
         ob . str ( "" );
       }  
      offset ++;           
    }
   if ( ! f . eof () )
    {
      f . close ();
      return ( false );
    }  
 
   f . close ();
    // pokud na posledni radce neco zbylo
   if ( offset % 16 != 0 )
    {
      cout << setfill ( ' ' ) << setw ( ( 16 - offset % 16 ) * 3 + 1 ) << "" << ob . str () << endl;
    } 
   return ( true ); 
 }


int main ( int argc, char * argv [] )
 {
   if ( argc != 2 ) 
    {
      cout << "Usage: dump <filename>" << endl;
      return ( 1 );
    }
    
   if ( ! dumpFile ( argv[1] ) )
    {
      cout << "Chyba prace se souborem." << endl;
      return ( 1 );
    } 

   return ( 0 ); 
 }

