/* reseni s C++ vlakny a mutexy
 */
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

int g_Cnt = 0;     /* nalezeno prvocisel */
int g_Pos = 0;     /* aktualne testovane prvocislo */
int g_Max;         /* mez - testujeme do max-1 */

mutex g_Mtx;

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
 */
void thrFunc ( void )
 {
   int localCnt = 0;
   /* Nalezena prvocisla budeme pocitat pouze do teto lokalni promenne,
    * po skonceni celeho vypoctu upravime (v zamcenem stavu) globalni pocitadlo.
    * Usetrime rezii na zbytecne zamykani a odemykani mutexu pri kazde inkrementaci.
    */
   while ( 1 )
    {
      int x;
      {
        unique_lock<mutex>  m ( g_Mtx );
        /* C++11 zamykani mutexu. Misto explicitniho m_Mtx . lock () a m_Mtx . unlock ()
         * lze vyuzit wrapper - objekt typu unique_lock. Tento pomocny objekt
         * zamkne ve svem konstruktoru predany mutex a v destruktoru jej
         * odemkne. Trik je, ze objekt deklarujeme pouze ve vnorenem bloku,
         * tedy zamek je zamceny prave v tomto bloku. Automaticke odemceni v destruktoru
         * zamek odemyka, tedy nezapomeneme jej zamceny (to je vyhodne zejmena pri hazeni vyjimek).
         */
        x = g_Pos ++;
      }

      if ( x >= g_Max ) break;
      if ( isPrime ( x ) ) localCnt ++;
    }
    {
      unique_lock<mutex>  m ( g_Mtx );
      /* Pouzijeme stejny mutex pro zaykani g_Pos i g_Cnt. Protoze pocet zamceni
       * pri uprave g_Cnt je maly (== pocet vlaken), neprineslo by rozdeleni do vice mutexu
       * zadne snizeni rezie.
       *
       */
      g_Cnt += localCnt;
    }
 }

int main ( int argc, char * argv [] )
 {
   int              thr;
   vector<thread>   threads;

   if ( argc != 3 || sscanf ( argv[1], "%d", &g_Max ) != 1 || sscanf ( argv[2], "%d", &thr ) != 1 )
    {
      printf ( "Usage: %s <max> <thr>\n", argv[0] );
      return ( 1 );
    }


   /* Vytvoreni vlaken - vlakno dostane svoji identifikaci (cele cislo).
    */
   for ( int i = 0; i < thr; i ++ )
    threads . push_back ( thread ( thrFunc ) );

   for ( auto & t : threads )
    t . join ();

   printf ( "Prvocisel < %d je %d\n", g_Max, g_Cnt );
   return ( 0 );
 }
