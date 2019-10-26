#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

/*
Reseni s garanci poradi vjezdu vlaku do nadrazi. Nektere scenare, ktere musime vyresit:

Scenar #1:
 - nadrazi neni plne, zadny vlak neceka ve fronte, pri vjezdu neni treba uspavat

Scenar #2:
 - nadrazi je plne, A ceka u nadrazi, B odjizidi
 - B uvolni kolej, unoznuje vjezd A
 - pokud by se mezi prepnutim kontextu B -> A objevil vlak C vjizdejici
   do nadrazi, je potreba jej uspat a dat prednost A (fronta)

Scenar #3:
 - nadrazi je plne, A a B cekaji ve fronte.
 - odjizdi C a D, uvolnuji sve koleje
 - je treba, aby A a B bylo probuzeno v poradi cekani a dale je potreba, aby
   se oba vlaky dostaly na nejake volne koleje (aby nadrazi bylo vyuzito).
*/



#define PLATFORM_MAX 3

/* Struktura popisujici cekajici vlaky (spojovy seznam)
 */

typedef struct TTrain
 {
    struct TTrain   * m_Next;
    pthread_cond_t    m_Cond;
    int               m_ID;
 } TTRAIN;

TTRAIN        * g_First = NULL; /* prvni vlak (nejblize nadrazi) */
TTRAIN        * g_Last  = NULL; /* posledni vlak (nejdale od nadrazi */
pthread_mutex_t g_Switch;       /* vyhybka - zamek pro kritickou sekci vyberu koleje */
int             g_Platform[PLATFORM_MAX]; /* priznaky obsazeni koleji */

/*---------------------------------------------------------------------------*/
/* Vypis cekajicich vlaku. Je rekurzivni, aby vypsana cisla vlaku sla zleva
 * (prijel posledni) doprava (nejdele cekajici).
 */
void displayQueue ( TTRAIN * t )
 {
   if ( ! t ) return;
   displayQueue ( t -> m_Next );
   printf ( "%d ", t -> m_ID );
 }
/*---------------------------------------------------------------------------*/
/* Vypis cekajicich vlaku + obsazeni koleji.
 */
void displayStation ( void )
 {
   int i;

   displayQueue ( g_First );
   printf ( "-> [" );
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
   if ( g_First || pl == PLATFORM_MAX )
    {
      /* Vlak musi cekat.
       */
      TTRAIN * w = (TTRAIN *) malloc ( sizeof ( *w ) );

      /* Struktura pro frontu cekajicich.
       */
      w -> m_Next = NULL;
      pthread_cond_init ( &w -> m_Cond, NULL );
      w -> m_ID = id;

      /* Zacleneni na konec fronty.
       */
      if ( g_Last )
        g_Last -> m_Next = w;
       else
        g_First = w;
      g_Last = w;
      /* Zobrazeni
       */
      displayStation ();
      /* Jsme vlastniky zamku, uspime se s timto zamkem na
       * sve vlastni podmince. Cekame dokud se nedostaneme na celo
       * fronty cekajicich a odjizdejici vlak nas neprobudi.
       *
       * Po dobu cekani bude zamek g_Switch odemknuty (to zajisti
       * pthread_cond_wait), tedy jina vlakna mohou manipulovat s obsahem
       * struktur. To je v poradku, jinak by vlakno nemohlo byt probuzeno.
       */
      do
       {
         pthread_cond_wait ( &w -> m_Cond, &g_Switch );
       } while ( (pl = findPlatform () ) == PLATFORM_MAX );

      /* Po probuzeni jsme opet zpet vlastniky zamku g_Switch.
       * To je dobre, muzeme ve strikturach provest potrebne upravy.
       */

      /* cekani na podminene promenne je v cyklu. Pro netere scenare by se mohlo stat, ze
       * se probudime v nevhodny okamzik:
       * Scenar #2 nahore:
       *    - protoze fronta byla neprazdna, C se uspal a ceka. Sanci vjet ma pouze A
       *    - pri vyjezdu A probudi za nim cekajici C -> kapacita nadrazi bude vyuzita
       * Scenar #3 nahore:
       *    - pokud se vlakna vystridaji v poradi: C -> D -> A pak se probouzi pouze A
       *      ten obsadi kolej na nadrazi. Je potreba, aby probudil priadne B za nim ve
       *      fronte, proto nize pridame probouzeni dalsiho ve fronte
       * Scenar #3 nahore:
       *    - pokud by ale doslo k probuzeni C -> A , pak nasledne probuzeni B
       *      selze (B neni kam umistit). Proto je volani cond_wait v cyklu, pokud probuzene
       *       vlakno zjisti, ze neni misto, opet se uspi. Tedt volani by bylo C -> A -> B (znovu uspat) -> D -> B
       *    - neni to 100% efektivni, ale je to sprane.
       */

      /* Odpojime se z fronty cekajicich
       */
      g_First = g_First -> m_Next;
      if ( g_First == NULL ) g_Last = NULL;
      /* Uvolneni prostredku.
       */
      pthread_cond_destroy ( &w -> m_Cond );
      free ( w );

      /* probudime dalsiho cekajiciho ve fronte (pokud existuje)
       * Jeho probuzeni nemusi byt uspesne (nebude volna kolej, tedy zase se uspi), ale
       * vzhledem ke scenari #3 nahore je to potreba zkusit
       */
      if ( g_First )
       pthread_cond_signal ( &g_First -> m_Cond );
    }
   /* Obsadit kolej
    */
   g_Platform[pl] = id;
   displayStation ();
   printf ( "Train %d loading/unloading, platform %d\n", id, pl);
   pthread_mutex_unlock ( &g_Switch );
   return ( pl );
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

   /* Pokud nejaky vlak ceka, probudime jej.
    */
   if ( g_First )
     pthread_cond_signal ( &g_First -> m_Cond );
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

   return ( 0 );
 }
 