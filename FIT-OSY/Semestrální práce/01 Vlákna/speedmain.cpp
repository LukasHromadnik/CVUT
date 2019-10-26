/*
 * Test sequential functions, measure speed (optimization disabled), does not test return values
 */
#include "solution.cpp"

//-------------------------------------------------------------------------------------------------
double             timeStamp                               ( void )
 {
   struct timeval tv;
   gettimeofday ( &tv, NULL );
   return tv . tv_sec + tv . tv_usec / 1000000.0;
 }
//-------------------------------------------------------------------------------------------------
void               costTest                                ( void )
 {
   const int RUNS = 7;
   int     sizeArr [RUNS] = { 10, 20, 40, 80, 90, 100, 140 };
   double  speedArr[RUNS] = { 0.000, 0.001, 0.015, 0.224, 0.358, 0.552, 2.072 };

   for ( int i = 0; i < RUNS; i ++ )
    {
      int size = sizeArr[i];
      int ** mat = new int * [size];

      for ( int y = 0; y < size; y ++ )
       {
         mat[y] = new int [size];
         for ( int x = 0; x < size; x ++ )
          mat[y][x] = (int)( -500 + 2000.0 * rand () / RAND_MAX );
       }
      double timeSt = timeStamp ();

      TRect  r;
      for ( int j = 0; j < 10; j ++ )
       FindByCost ( mat, size, (int)(500.0 * size * size * rand () / RAND_MAX ), &r );

      printf ( "Cost (%d) -> %.3f, ref %.3f\n", size, (timeStamp () - timeSt ) / 10, speedArr[i] );

      for ( int y = 0; y < size; y ++ )
       delete [] mat[y];
      delete [] mat;
    }
 }
//-------------------------------------------------------------------------------------------------
void               crimeTest                               ( void )
 {
   const int RUNS = 7;
   int     sizeArr [RUNS] = { 10, 30, 90, 150, 250, 400, 600 };
   double  speedArr[RUNS] = { 0.000, 0.000, 0.005, 0.023, 0.113, 0.402, 1.537 };

   for ( int i = 0; i < RUNS; i ++ )
    {
      int size = sizeArr[i];
      double ** mat = new double * [size];

      for ( int y = 0; y < size; y ++ )
       {
         mat[y] = new double [size];
         for ( int x = 0; x < size; x ++ )
          mat[y][x] = 1000.0 * rand () / RAND_MAX;
       }
      double timeSt = timeStamp ();

      TRect  r;
      for ( int j = 0; j < 10; j ++ )
       FindByCrime ( mat, size, 1000.0 * rand () / RAND_MAX, &r );

      printf ( "Crime (%d) -> %.3f, ref %.3f\n", size, (timeStamp () - timeSt ) / 10, speedArr[i] );

      for ( int y = 0; y < size; y ++ )
       delete [] mat[y];
      delete [] mat;
    }
 }
//-------------------------------------------------------------------------------------------------
int                main                                    ( void )
 {
   costTest ();
   crimeTest ();
   return 0;
 } 
