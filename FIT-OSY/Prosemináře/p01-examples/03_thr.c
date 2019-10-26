#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Paralelni radeni pole vice vlakny. Velke pole je rozdeleno na useky 
 * stejne delky, kazdy usek je setriden algoritmem bud quick nebo select sort (tedy ne optimalne!) 
 * vlastnim vlaknem. Po dokonceni prace vlaken se setridena pole sleji 
 * dohromady operaci merge (toto je jiz sekvencni).
 * 
 * Program ukazuje vytvoreni vlaken, ukoncovani, predavani parametru. Dale 
 * umoznuje zkoumat doby behu - celkovy cas (wall time) a strojovy cas.
 * 
 * Je videt:
 * - program s jednim vlaknem se chova jako sekvencni razeni,
 * - pokles celkoveho casu pri zvyseni poctu vlaken. Pokles je umocnen pokud
 *   se radi neefektivne (select sort), ale sleva efektivne (aka merge sort)
 * - pokud je razeni ve vlaknech nahrazeno quick sort algoritmem, je videt pokles 
 *   celkoveho casu za cenu mirneho rustu strojoveho casu
 * - je videt rust celkoveho casu, pokud je vlaken velmi mnoho a prevazi sekvencni cast + rezie 
 *   vytvareni vlaken.
 */

/* Parametry pro praci vlakna
 */
typedef struct TThrArg 
 {
   pthread_t   m_ThrID;  /* id vlakna */
   int         m_Cnt;    /* pocet prvku ke razeni */
   int       * m_Data;   /* pole k razeni */
 } TTHRARG;

/* #define SELECT_SORT */

/*---------------------------------------------------------------------------*/
int intSort ( int * a, int * b )
 {
   return ( *a - *b );
 }
/*---------------------------------------------------------------------------*/
void * thrFunc ( void * arg )
 {
   TTHRARG  * data = (TTHRARG *)arg;
#ifdef SELECT_SORT
   int        i, j, n, tmp, *a, min;
   
   n = data -> m_Cnt;
   a = data -> m_Data;
   /* razeni algoritmem select sort */
   for ( i = 0; i < n; i ++ )
    {
      min = i;
      for ( j = i + 1; j < n; j ++ )
       if ( a[j] < a[min] ) min = j;
  
      tmp = a[min];
      a[min] = a[i];
      a[i] = tmp;
    }
#else /* SELECT_SORT */
   qsort ( data -> m_Data, data -> m_Cnt, sizeof ( *data -> m_Data ), (int (*)(const void *, const void *)) intSort );    
#endif /* SELECT_SORT */
   return ( NULL );
 }
/*---------------------------------------------------------------------------*/
int main ( int argc, char * argv [] )
 {
   TTHRARG      * arg;
   pthread_attr_t thrAttr;
   int            i, thr, min, block, max, * src, *dst, * tmp, ofs;
   void         * dummy;

   if ( argc != 3 || sscanf ( argv[1], "%d", &max ) != 1 ||
        sscanf ( argv[2], "%d", &thr ) != 1 )
    {
      printf ( "Usage: %s <max> <thr>\n", argv[0] );
      return ( 1 );
    }

   arg = (TTHRARG *) malloc ( thr * sizeof ( *arg ) );
   src = (int*) malloc ( sizeof ( *src ) * max );
   dst = (int*) malloc ( sizeof ( *dst ) * max );

   /* vyplnit nahodnymi hodnotami */
   for ( i = 0; i < max; i ++ )
    src[i] = rand () % 10000;

   /* rozdelime pole na casti, rovnomerne (max/thr na jedno vlakno).
    * posledni vlakno dostane trochu vice - co zbyde (ternarni operator)
    */ 
   block = max / thr;
   ofs   = 0;
   for ( i = 0; i < thr; i ++ )
    {
      arg[i] . m_Data = src + ofs;
      arg[i] . m_Cnt  = i == thr - 1 ? max - ofs : block;
      ofs += block;
    }  

   /* paralelni cast - kazde vlakno radi svuj usek pole
    */
   pthread_attr_init ( &thrAttr );
   pthread_attr_setdetachstate ( &thrAttr, PTHREAD_CREATE_JOINABLE );
   for ( i = 0; i < thr; i ++ )
    if ( pthread_create ( &arg[i] . m_ThrID, &thrAttr, thrFunc, &arg[i] ) )
     {
       free ( arg );
       free ( src );
       free ( dst );
       perror ( "Create thread\n" );
       pthread_attr_destroy ( &thrAttr );
       return ( 1 );
     }
   pthread_attr_destroy ( &thrAttr );

   /* cekani na dokonceni 
    */ 
   for ( i = 0; i < thr; i ++ )
    pthread_join ( arg[i] . m_ThrID, &dummy );

   /* spojovani z thr casti do ciloveho pole. Jedna se o merge
    * s vice vstupy (thr vstupu)
    */
   tmp = dst;
   while ( 1 )
    {
      min = -1;
      for ( i = 0; i < thr; i ++ )
       if ( arg[i] . m_Cnt )
        if ( min == -1 || *arg[i] . m_Data < *arg[min] . m_Data ) min = i;
      /* min udava pole, ktere obsahuje nejmensi prvek
       */ 
      if ( min == -1 ) break;
      
      *tmp ++ = *arg[min] . m_Data ++;     
      arg[min] . m_Cnt --;
    }
/* vypis setrideneho pole, pro mereni casu odpojeno
 */
/*  
   for ( i = 0; i < max; i ++ )
    printf ( "%d ", dst[i] );
   printf ( "\n" );
*/   
   free ( arg );
   free ( src );
   free ( dst );
   
   return ( 0 ); 
 }
