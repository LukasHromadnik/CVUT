#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

/*
  reseni bez garance poradi, s podminenymi promennymi
*/

#define PLATFORM_MAX 3


pthread_cond_t  g_Wait;
pthread_mutex_t g_Switch;       /* vyhybka - zamek pro kritickou sekci vyberu koleje */
int             g_Platform[PLATFORM_MAX]; /* priznaky obsazeni koleji */

/*---------------------------------------------------------------------------*/
/* Vypis cekajicich vlaku + obsazeni koleji.
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
/* Najde volnou kolej, vrati jeji cislo. Predpoklada, ze volajici vlakno
 * vlastni zamek g_Switch.
 */
int findPlatform ( void )
 {
   int i;

   for ( i = 0; i < PLATFORM_MAX; i ++ )
    if ( g_Platform[i] == 0 )
     return ( i );

   return ( PLATFORM_MAX );
 }
/*---------------------------------------------------------------------------*/
/* Prijezd do nadrazi
 *
 */
int waitStation ( int id )
 {
   int pl;

   pthread_mutex_lock ( &g_Switch );
   printf ( "Train %d arriving\n", id );
   pl = findPlatform ();

   /* Vjezd pouze pro alespon jednu volnou kolej a prazdnou frontu cekajicich
    * resi scenar #1 a #2 nahore
    */
   if ( pl == PLATFORM_MAX )
    {
      /* Vlak musi cekat.
       */
      displayStation ();
      /* Jsme vlastniky zamku, uspime se na podminene promenne
       */
      do
       {
         pthread_cond_wait ( &g_Wait, &g_Switch );
       } while ( (pl = findPlatform () ) == PLATFORM_MAX );

      /* Po probuzeni jsme opet zpet vlastniky zamku g_Switch.
       * To je dobre, muzeme ve strikturach provest potrebne upravy.
       */
    }
   /* Obsadit kolej
    */
   g_Platform[pl] = id;
   displayStation ();
   printf ( "Train %d loading/unloading, platform %d\n", id, pl);
   pthread_mutex_unlock ( &g_Switch );
   return pl;
 }
/*---------------------------------------------------------------------------*/
void leaveStation ( int platform )
 {
   /* Odjezd z nadrazi
    */
   pthread_mutex_lock ( &g_Switch );
   printf ( "Train %d departed, platform %d now empty\n", g_Platform[platform], platform );
   /* Uvolnime kolej
    */
   g_Platform[platform] = 0;
   displayStation ();

   /* Probudime pripadne cekajici na podminene promenne.
    * Pokud nikdo neceka, nic se nedeje (signal je v te chvili no-operation).
    */
   pthread_cond_signal ( &g_Wait );
   pthread_mutex_unlock ( &g_Switch );
 }
/*---------------------------------------------------------------------------*/
/* Nahodne cislo od min do max.
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
   pthread_cond_init ( &g_Wait, NULL );

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
   pthread_mutex_destroy ( &g_Switch );
   pthread_cond_destroy ( &g_Wait );

   return ( 0 );
 }
 