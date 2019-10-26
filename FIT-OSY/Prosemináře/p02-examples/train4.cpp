/* reseni s C++11 vlakny, mutexy a cond. var
 */
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <deque>
#include <unistd.h>
using namespace std;

const int PLATFORM_MAX = 3;
struct CTrain
 {
                       CTrain ( int id )
                        : m_ID ( id ), m_Platform ( PLATFORM_MAX ) { }
   condition_variable  m_Cond;
   int                 m_ID;
   int                 m_Platform;
 };

// Do fronty cekajicich nechceme ukladat kopie CTrain, ale odkazy (reference) na
// existujici objekty. Obecne, neni dobry napad kopirovat objekty typu thread/mutex/cond_var.
// Kompilator se tomu brani, prislusne copy konstruktory jsou deleted.
// Bud muzeme ukladat ukazatele, nebo reference v podobe reference-wrapperu
deque<reference_wrapper<CTrain> >  g_Waiting;
mutex          g_Switch;                 /* vyhybka - zamek pro kritickou sekci vyberu koleje */
int            g_Platform[PLATFORM_MAX]; /* priznaky obsazeni koleji */

/*---------------------------------------------------------------------------*/
/* Vypis cekajicich vlaku + obsazeni koleji.
 */
void displayStation ( void )
 {
   // display in reverse order
   for ( auto it = g_Waiting . rbegin (); it != g_Waiting . rend (); it ++  )
    printf ( " %2d ", it -> get () . m_ID );

   printf ( "-> [" );
   for ( int i = 0 ; i < PLATFORM_MAX; i ++ )
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
   for ( int i = 0; i < PLATFORM_MAX; i ++ )
    if ( g_Platform[i] == 0 )
     return i;

   return PLATFORM_MAX;
 }
/*---------------------------------------------------------------------------*/
/* Prijezd do nadrazi
 *
 */
int waitStation ( int id )
 {
   unique_lock<mutex> m ( g_Switch );
   printf ( "Train %d arriving\n", id );

   int pl = findPlatform ();
   if ( pl == PLATFORM_MAX || g_Waiting . size () )
    {
      // Nadrazi je obsazeno. Vlak musi cekat.
      CTrain myself ( id );
      g_Waiting . push_back ( myself );

      // Zobrazeni
      displayStation ();

      // uspani & odemceni zamku, pri probuzeni kontrolujeme, ze jsme opravdu prvni na rade.
      // (Mohlo by se stat,
      // A pro extra spatne planovani by se mohlo stat, ze druhy probuzeny cekajici vlak
      // ziska cpu drive nez prvni.
      myself . m_Cond . wait ( m, [&pl, id] (void)
       {
         return ( pl = findPlatform () ) != PLATFORM_MAX
                  && g_Waiting . front () . get () . m_ID == id; // tedy pokud nejsme prvni na rade nebo neni volno, zase se uspavame
       } );

      // mame jistotu, ze odstranujeme sebe z fronty cekajicich
      g_Waiting . pop_front ();

      // nepravdepodobne, ale mozne. Pokud by se podarilo probudit najednou dva cekajici vlaky
      // a druhy ve fronte by se dostal k CPU drive nez prvni, pak se opet uspal (podminka u cond_wait)
      // v takovem pripade by jej ale jiz nikdo neprobudil. Tedy z vlakna vlaku, ktery vjizdi na nadrazi
      // jeste probudime pripadny dalsi vlak za nami ve fronte. Pokud dalsi vlak najde volnou kolej, obsadi ji
      // (a pripadne probouzi i dalsi, ...)
      if ( g_Waiting . size () )
       g_Waiting . front () . get () . m_Cond . notify_one ();
    }
   // stale vlastnime mutex, tedy bezpecne obsazujeme kolej
   g_Platform[pl] = id;

   displayStation ();
   printf ( "Train %d loading/unloading, platform %d\n", id, pl);
   return pl;
 }
/*---------------------------------------------------------------------------*/
void leaveStation ( int platform )
 {
   // Odjezd z nadrazi
   unique_lock<mutex> m ( g_Switch );
   printf ( "Train %d departed, platform %d now empty\n", g_Platform[platform], platform );
   // Uvolnime kolej
   g_Platform[platform] = 0;
   displayStation ();

   if ( g_Waiting . size () > 0 ) // Pokud nejaky vlak ceka, probudime jej.
    g_Waiting . front () . get () . m_Cond . notify_one ();
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
 */
void train ( int id )
 {
   while ( 1 )
    {
      int platform = waitStation ( (int) id );
      // Nakladka/vykladka
      usleep ( 1000 * randomRange ( 1000, 5000 ) );

      leaveStation ( platform );
      // Jizda vlaku
      usleep ( 1000 * randomRange ( 2000, 10000 ) );
    }
 }
/*---------------------------------------------------------------------------*/
int main ( int argc, char * argv [] )
 {
   int thr;
   vector<thread> trains;

   if ( argc != 2 || sscanf ( argv[1], "%d", &thr ) != 1 )
    {
      printf ( "usage: %s <thr>\n", argv[0] );
      return ( 1 );
    }

   for ( int i = 0; i < thr; i ++ )
    trains . push_back ( thread ( train, i + 1 ) );

   for ( auto & t : trains )
    t . join ();

   return 0;
 }
 