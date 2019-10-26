#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t g_Mutex;

int counter = 0;

typedef struct TThrArg 
 {
   int        m_Max;
   int        m_Num;
   pthread_t  m_ThrID;
 } TTHRARG;
 
/* Funkce vlakna - vypise cisla v danem rozsahu. Parametry dostane 
 * jako netypovy ukazatel void *. Protoze vlakno vytvarime a vime,
 * ze tento netypovy ukazatel ve skutecnosti ukazuje na nami pripravenou
 * strukturu TTHRARG, pretypujeme tento ukazatel a pak pomoci typovaneho
 * ukazatele data pracujeme s predanymi hodnotami.
 */
void * thrFunc ( void * arg )
 {
   int i, l = 0;
   TTHRARG  * data = (TTHRARG *)arg;
   
   for ( i = 0; i <= data -> m_Max; i ++ ) {
   	l += i;
   }
   	pthread_mutex_lock(&g_Mutex);
   	counter += l;
   	pthread_mutex_unlock(&g_Mutex);
    /* printf ( "%d: %d\n", data -> m_Num, i ); */
   return ( NULL );
 }

int main ( int argc, char * argv [] )
 {
   TTHRARG      * arg;
   pthread_attr_t thrAttr;
   int            i, thr;
   void         * dummy;

   /* Vytvarime vlakno pro kazdy parametr z prikazove radky
    */
   thr = argc - 1;
   if ( thr <= 0 )
    {
      printf ( "Usage: %s n1 [n2 [n3 [...]]]\n", argv[0] );
      return ( 1 );
    }

   /* Pole parametru pro vlakna. 
    */
   arg = (TTHRARG *) malloc ( thr * sizeof ( *arg ) );

   for ( i = 0; i < thr; i ++ )
    {
      if ( sscanf ( argv[i+1], "%d", &arg[i] . m_Max) != 1 )
       {
         free ( arg );
         printf ( "Invalid argument %d\n", i + 1 );
         return ( 1 );
       }
      arg[i] . m_Num = i;
    }

    pthread_mutex_init( &g_Mutex, NULL );
 
   /* Atributy vlakna (ciste reseni, na vlakno lze cekat pomoci pthread_join). 
    */ 
   pthread_attr_init ( &thrAttr );
   pthread_attr_setdetachstate ( &thrAttr, PTHREAD_CREATE_JOINABLE );
   
   /* Vytvareni vlaken
    */  
   for ( i = 0; i < thr; i ++ )
    if ( pthread_create ( &arg[i] . m_ThrID, &thrAttr, thrFunc, &arg[i] ) )
     {
       free ( arg );
       perror ( "Create thread\n" );
       pthread_attr_destroy ( &thrAttr );
       return ( 1 );
     }
   pthread_attr_destroy ( &thrAttr );
 
   /* Cekani na dokonceni
    */
   for ( i = 0; i < thr; i ++ )
    pthread_join ( arg[i].m_ThrID, &dummy );

	printf("%d\n", counter);

   /* Uklid
    */
   free ( arg );
   pthread_mutex_destroy( &g_Mutex );
   return ( 0 ); 
 }
