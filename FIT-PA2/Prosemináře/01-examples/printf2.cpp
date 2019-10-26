/* Jednoducha reimplementace funkce printf. Ma pouze omezenou funkcionalitu,
 * zvlada pouze konverze %d, %s a %%. Demonstruje, ze kompilator pri pouziti 
 * vypustky nemuze rozumet formatovacimu retezci, tedy neni nas obecne schopen 
 * informovat o chybach v parovani skutecny parametr - konverze.
 */
#include <cstdarg>
#include <cstdio>
using namespace std;

static void  printInt ( int x )
 {
   if ( x <= 9 ) 
     putc ( x + '0', stdout );
    else
     {
       printInt ( x / 10 );
       putc ( x % 10 + '0', stdout );
     } 
 }

void myprintf ( const char * fmt, ... )
 {
   va_list va;
   bool    conv = false;
   int     i;
   const char * str;
   
   va_start ( va, fmt );
   
   while ( *fmt )
    {
      if ( conv )
       {
         switch ( *fmt )
          {
            case 'd':
             i = va_arg ( va, int );
             if ( i < 0 ) 
              {
                putc ( '-', stdout );
                i = -i;
              }
             printInt ( i );
             break;
            case 's':
             str = va_arg ( va, const char *);
             while ( *str )
              {
                putc ( *str, stdout );
                str ++;
              }
             break;
            case '%':
             putc ( '%', stdout );
             break;
            default:
             break;
          }
         conv = false;
       }
      else
       {
         if ( *fmt == '%' ) 
           conv = true;
          else
           putc ( *fmt, stdout ); 
       }
      fmt ++;
    }
   va_end ( va );
 }

int main ( int argc, char * argv [] )
 {
   int          a = 10;
   const char * c = "test";

   
   myprintf ( "a = %d\n", a );
   myprintf ( "c = %s\n", c );

   // Pozor, neparuje typ argumentu / konverze   
   myprintf ( "c = %d\n", c );
   myprintf ( "a = %s\n", a );     

   return ( 0 ); 
 }
