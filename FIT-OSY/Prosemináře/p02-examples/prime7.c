/* NESPRAVNE reseni - obsahuje deadlock. Deadlock se projevi zejmena na
 * viceprocesorovych systemech.
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
/* Mame 2 mutexy, ponekud paranoidne budeme zamykat vzdy oba pri vstupu
 * do kriticke sekce.
 */
pthread_mutex_t g_Mutex1, g_Mutex2;


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
      /* Prvni kriticka sekce, zamykana obema mutexy.
       * Je to pouze demonstrace, samozrejme by stacil
       * pouze jeden mutex. Zamykame v poradi M1 M2.
       */
      pthread_mutex_lock ( &g_Mutex1 );
      pthread_mutex_lock ( &g_Mutex2 );
      x = g_Pos++;
      /* Na poradi odemykani nezalezi (odemykani neni blokujici).
       */
      pthread_mutex_unlock ( &g_Mutex2 );
      pthread_mutex_unlock ( &g_Mutex1 );

      if ( x >= g_Max ) break;
      if ( isPrime ( x ) ) 
       {
         /* Druha kriticka sekce. Opet zamykame dvema
          * mutexy (opet zbytecne). Navic tentokrat zamykame v poradi
          * M2 M1 (opacne nez v predchozi kriticke sekci). Tim vzniknou podminky pro
          * deadlock.
          */
         pthread_mutex_lock ( &g_Mutex2 );
         pthread_mutex_lock ( &g_Mutex1 );
         g_Cnt ++;
         /* Na poradi odemykani nezalezi (odemykani neni blokujici).
          */
         pthread_mutex_unlock ( &g_Mutex1 );
         pthread_mutex_unlock ( &g_Mutex2 );
       }  
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

   pthread_mutex_init ( &g_Mutex1, NULL );
   pthread_mutex_init ( &g_Mutex2, NULL );

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
   pthread_mutex_destroy ( &g_Mutex1 );
   pthread_mutex_destroy ( &g_Mutex2 );
     
   printf ( "Prvocisel < %d je %d\n", g_Max, g_Cnt );
   return ( 0 );
 } 
