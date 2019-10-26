/* Sekvencni reseni. Spravne, dost pomale.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* Naivni algoritmus testu prvociselnosti. Testuje delitele do sqrt(x),
 * pri prvnim nalezenem deliteli vraci "neni prvocislo".
 * Kod by sel vylepsit (napr. pro suda cisla), ale to zde neni ucelem.
 */

int isPrime ( int x )
 {
   int max, i;
   
   if ( x < 2 ) return ( 0 );
   max = (int)sqrt ( x );
   for ( i = 2; i <= max; i ++ )
    if ( x % i == 0 ) 
     return ( 0 );
   return ( 1 );
 }

int main ( int argc, char * argv [] )
 {
   int      thr, max, cnt = 0, i;
   
   if ( argc != 3 || sscanf ( argv[1], "%d", &max ) != 1 || sscanf ( argv[2], "%d", &thr ) != 1 )
    {
      printf ( "Usage: %s <max> <thr>\n", argv[0] );
      return ( 1 );
    }
    
/* pro kazdeho kandidata otestovat prvociselnost,
 * pripadne zapocitat.
 */
   for ( i =0 ; i < max; i ++ )
    if ( isPrime ( i ) )
     cnt ++;
     
   printf ( "Prvocisel < %d je %d\n", max, cnt );
   return ( 0 );
 } 
