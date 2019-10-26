#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Funkce vlakna. Zde se zacne provadet nove vytvorene vlakno.
 */
void * thrFunc ( void * arg )
 {
   int i;
   
   for ( i = 0; i < 100; i ++ )
    printf ( "%d\n", i );
   return ( NULL );
 }

int main ( int argc, char * argv [] )
 {
   pthread_t      thr[3];
   pthread_attr_t thrAttr;
   int            i;
   void         * dummy;

   /* Atribut vlakna, aby slo pouzit pthread_join
    */
    
   pthread_attr_init ( &thrAttr );
   pthread_attr_setdetachstate ( &thrAttr, PTHREAD_CREATE_JOINABLE );
   
   /* Vytvoreni vlaken
    */
   for ( i = 0; i < 3; i ++ )
    if ( pthread_create ( &thr[i], &thrAttr, thrFunc, NULL ) )
     {
       perror ( "Create thread\n" );
       return ( 1 );
     }
   pthread_attr_destroy ( &thrAttr );
 
   /* Cekani na dokonceni vlaken
    */
   for ( i = 0; i < 3; i ++ )
    pthread_join ( thr[i], &dummy );
 
   return ( 0 ); 
 }
