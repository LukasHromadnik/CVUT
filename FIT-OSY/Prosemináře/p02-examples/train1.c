#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define PLATFORM_MAX 3

sem_t           g_Sem;        /* semafor - pocet volnych koleji */
pthread_mutex_t g_Switch;     /* mutex vyhybky - pro pristup do priznaku obsazeni koleji */
int             g_Platform[PLATFORM_MAX]; /* obsazeni koleji (0= volna, 1-n ID vlaku) */

/*---------------------------------------------------------------------------*/
/* Zobrazeni obsazeni koleji nadrazi
 */
void displayStation ( void )
 {
   int i;

   printf ( "[" );
   for ( i = 0 ; i < PLATFORM_MAX; i ++ )
    {
      if ( g_Platform[i] )
        printf ( " %2d ", g_Platform[i] );
       else
        printf ( " -- " );
    }
   printf ( "]\n" ); 
 }
/*---------------------------------------------------------------------------*/
/* Prijezd vlaku do nadrazi
 */
int waitStation ( int id )
 {
   int i;
   
   printf ( "Train %d arriving\n", id );
   /* Cekani na uvolneni koleje
    */
   sem_wait ( &g_Sem );
   pthread_mutex_lock ( &g_Switch );

   /* Hledame volnoi kolej
    */
   for ( i = 0; i < PLATFORM_MAX; i ++ )
    if ( g_Platform[i] == 0 )
     break;
   /* Vime, ze nejmene jedna kolej musi byt volna - jinak by nas se semafor nepustil.
    * Obsadime ji.
    */
   g_Platform[i] = id;
   displayStation ();
   printf ( "Train %d loading/unloading, platform %d\n", id, i );
   pthread_mutex_unlock ( &g_Switch );
   return ( i );
 }
/*---------------------------------------------------------------------------*/
/* Odjezd ze stanice
 */
void leaveStation ( int platform )
 {
   pthread_mutex_lock ( &g_Switch );
   /* Uvolnime kolej (vlozime 0)
    */
   printf ( "Train %d departed, platform %d now empty\n", g_Platform[platform], platform );
   g_Platform[platform] = 0;
   displayStation ();
   pthread_mutex_unlock ( &g_Switch );
   /* Zvysime semafor na prijezdu.
    */
   sem_post ( &g_Sem );
 }
/*---------------------------------------------------------------------------*/
/* Generuje nahodne cislo v rozsahu min - max
 */
int randomRange ( int min, int max )
 {
   return (int)( min + 1.0 * rand () / RAND_MAX * ( max - min ));
 }
/*---------------------------------------------------------------------------*/
/* Funkce vlakna - simulator vlaku.
 * Vlakno dostane parametrem svuj identifikator - cislo od 1 do poctu vlaku
 * (predavame intptr_t -> integer velikosti ukazatele na dane platforme).
 */
void * train ( uintptr_t id )
 {
   int platform;
   while ( 1 )
    { 
      platform = waitStation ( (int) id );
      /* Nakladka/vykladka
       */
      usleep ( 1000 * randomRange ( 1000, 5000 ) );
      leaveStation ( platform );
      /* Jizda vlaku
       */
      usleep ( 1000 * randomRange ( 2000, 10000 ) );
    }
   return ( NULL ); 
 }
/*---------------------------------------------------------------------------*/
int main ( int argc, char * argv [] )
 {
   pthread_t * thrID;
   int thr, i;
   pthread_attr_t attr;
   
   if ( argc != 2 || sscanf ( argv[1], "%d", &thr ) != 1 )
    {
      printf ( "usage: %s <thr>\n", argv[0] );
      return ( 1 );
    }

   pthread_mutex_init ( &g_Switch, NULL );
   /* Semafor ma hodnotu PLATFORM_MAX a neni sdilen mezi procesy (parametr 2 je 0)
    */
   sem_init ( &g_Sem, 0, PLATFORM_MAX );

   pthread_attr_init ( &attr );
   pthread_attr_setdetachstate ( &attr, PTHREAD_CREATE_JOINABLE );
   
   thrID = (pthread_t *) malloc ( sizeof ( *thrID ) * thr );
   for ( i = 0; i < thr; i ++ )
    if ( pthread_create ( &thrID[i], &attr, (void *(*)(void*)) train, (void*)(uintptr_t) (i + 1) ) )
     {
       perror ( "pthread_create" );
       return ( 1 );
     }
   pthread_attr_destroy ( &attr );
   
   for ( i = 0; i < thr; i ++ )
    pthread_join ( thrID[i], NULL );

   free ( thrID ); 
   sem_destroy ( &g_Sem );
   pthread_mutex_destroy ( &g_Switch );
 
   return ( 0 );
 }
 