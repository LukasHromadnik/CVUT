/* NESPRAVNE reseni se zamykanim mutexy. Mutexy funguji spravne, ale jsou spatne pouzite.
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>


int g_Cnt = 0;     /* nalezeno prvocisel */
int g_Pos = 0;     /* aktualne testovane prvocislo */
int g_Max;         /* mez - testujeme do max-1 */
pthread_mutex_t g_Mutex; /* mutex zamykajici pristup ke g_Cnt a g_Pos */


/* Test prvociselnosti - naivni.
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
 
/* Funkce vlakna - vezme dalsi cislo k otestovani, otestuje,
 * upravi pocitadlo.
 * Vlakno dostane parametrem svuj identifikator - cislo od 1 do poctu vlaken
 * (predavame intptr_t -> integer velikosti ukazatele na dane platforme).
 */
void * thrFunc ( intptr_t myID )
 {
   int x;
   
   while ( 1 )
    {
      /* Zamykame atomicky, do kriticke sekce se dostane jen jedno vlakno.
       * To je spravne, ale kriticka sekce je zde zbytecne dlouha.
       * Efektivne n - 1 vlaken ceka na zamceni a 1 vlakno pocita.
       * Slozite tedy implementujeme reseni, ktere bezi stejne rychle jako
       * reseni sekvencni.
       */
      pthread_mutex_lock ( &g_Mutex );
      x = g_Pos++;
      if ( x >= g_Max )
       {
         pthread_mutex_unlock ( &g_Mutex );
         break;
       }   
      if ( isPrime ( x ) ) g_Cnt ++;
      pthread_mutex_unlock ( &g_Mutex );
    }
   return ( NULL );
 } 

int main ( int argc, char * argv [] )
 {
   int              thr, i;
   pthread_t      * thrID;
   pthread_attr_t   attr;
   
   
   if ( argc != 3 || sscanf ( argv[1], "%d", &g_Max ) != 1 || sscanf ( argv[2], "%d", &thr ) != 1 )
    {
      printf ( "Usage: %s <max> <thr>\n", argv[0] );
      return ( 1 );
    }
    
   /* Pole identifikatoru vlaken
    */
   thrID = (pthread_t *) malloc ( sizeof ( *thrID ) * thr );

   pthread_mutex_init ( &g_Mutex, NULL );

   pthread_attr_init ( &attr );
   pthread_attr_setdetachstate ( &attr, PTHREAD_CREATE_JOINABLE );

   /* Vytvoreni vlaken - vlakno dostane svoji identifikaci (cele cislo).
    */
   for ( i = 0; i < thr; i ++ )
    if ( pthread_create ( &thrID[i], &attr, (void*(*)(void*)) thrFunc, (void*)(intptr_t) (i+1) ) )
     {
       perror ( "pthread_create" );
       return ( 1 );    
     }
   pthread_attr_destroy ( &attr );  
     
   for ( i = 0; i < thr; i ++ )
    pthread_join ( thrID[i], NULL );  

   free ( thrID );
   pthread_mutex_destroy ( &g_Mutex );
     
   printf ( "Prvocisel < %d je %d\n", g_Max, g_Cnt );
   return ( 0 );
 } 
