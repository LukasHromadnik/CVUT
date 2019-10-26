#include <cstdio>
#include <cstdlib>
#include <thread>
#include <vector>
#include <algorithm>
using namespace std;

/* Paralelni radeni pole vice vlakny. Velke pole je rozdeleno na useky
 * stejne delky, kazdy usek je setriden algoritmem bud quick nebo select sort (tedy ne optimalne!)
 * vlastnim vlaknem. Po dokonceni prace vlaken se setridena pole sliji
 * dohromady operaci merge (toto je jiz sekvencni).
 *
 * Verze vyuziva C++11 thread API
 */

#define SELECT_SORT

/*---------------------------------------------------------------------------*/
void   thrFunc ( int * data, int n )
 {
#ifdef SELECT_SORT
   /* razeni algoritmem select sort */
   for ( int i = 0; i < n; i ++ )
    {
      int min = i;
      for ( int j = i + 1; j < n; j ++ )
       if ( data[j] < data[min] ) min = j;
      swap ( data[i], data[min] );
    }
#else /* SELECT_SORT */
   sort ( data, data + n, [] ( const int & a, const int & b ) { return a < b; } );
#endif /* SELECT_SORT */
 }
/*---------------------------------------------------------------------------*/
void   merge ( int * & src, int * & dst, int n, int thr )
 {
   int block = n / thr;
   while ( block < n )
    {
      /* implementace merge muze rozdelit posledni delsi serazeny blok a zbytecne jej
       * zase spojit. z hlediska funkce to nevadi, rozdeleni serazeneho bloku zachovava
       * vlastnost "byt serazeny"
       */
      for ( int i = 0;  i < n; i += 2 * block )
       merge ( src + i,  src + min ( i + block, n ),
               src + min ( i + block, n ), src + min(i + 2*block, n ),
               dst + i );
      swap ( src, dst );
      block *= 2;
    }
   swap ( src, dst );
 }
/*---------------------------------------------------------------------------*/
int main ( int argc, char * argv [] )
 {
   vector<thread>  threads;
   int thr, max;

   if ( argc != 3 || sscanf ( argv[1], "%d", &max ) != 1 ||
        sscanf ( argv[2], "%d", &thr ) != 1 )
    {
      printf ( "Usage: %s <max> <thr>\n", argv[0] );
      return ( 1 );
    }

   int * src = new int [ max ];
   int * dst = new int [ max ];

   /* vyplnit nahodnymi hodnotami */
   for ( int i = 0; i < max; i ++ )
    src[i] = rand () % 10000;

   int block = max / thr;

   /* paralelni cast - kazde vlakno radi svuj usek pole
    */
   for ( int i = 0; i < thr; i ++ )
    threads . push_back ( thread ( thrFunc, src + i * block, i == thr - 1 ? max - i * block : block ) );

   /* cekani na dokonceni
    */
   for ( int i = 0; i < thr; i ++ )
    threads[i] . join ();

   merge ( src, dst, max, thr );

/* vypis serazeneho pole, pro mereni casu odpojeno
 */
/*
   for ( int i = 0; i < max; i ++ )
    printf ( "%d ", dst[i] );
   printf ( "\n" );
*/
   delete [] src;
   delete [] dst;
   return ( 0 );
 }
