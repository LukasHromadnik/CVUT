/*
 * interface, does not validate results
 */
#include "solution.cpp"
 
pthread_mutex_t    g_Mtx;
int                g_Cost, g_Crime;

//-------------------------------------------------------------------------------------------------
void               costNotify                              ( const TCostProblem * p,
                                                             const TRect     * r )
 {
   printf ( "costNotify\n" );
   for ( int i = 0; i < p -> m_Size; i ++ )
    delete [] p -> m_Values[i];
   delete [] p -> m_Values;
   delete p;
 }                                                             
//-------------------------------------------------------------------------------------------------
const TCostProblem * costGeneratorFunc                     ( void )
 {
   printf ( "costGen\n" );
   pthread_mutex_lock ( &g_Mtx );
   bool gen = g_Cost-- > 0;
   pthread_mutex_unlock ( &g_Mtx );
   
   if ( ! gen ) return NULL;
   
   TCostProblem * p = new TCostProblem;
   
   int size = (int)( 10 + 100.0 * rand () / RAND_MAX );
   int ** mat = new int * [size];
   for ( int y = 0; y < size; y ++ )
    {
      mat[y] = new int [size];
      for ( int x = 0; x < size; x ++ )
       mat[y][x] = (int)( -500 + 2000.0 * rand () / RAND_MAX );
    }
  
   p -> m_Values  = mat;
   p -> m_Size    = size;
   p -> m_MaxCost = (int)(500.0 * size * size * rand () / RAND_MAX );
   p -> m_Done    = costNotify;
   return p;
 }
//-------------------------------------------------------------------------------------------------
void               crimeNotify                             ( const TCrimeProblem * p,
                                                             const TRect     * r )
 {
   printf ( "crimeNotify\n" );

   for ( int i = 0; i < p -> m_Size; i ++ )
    delete [] p -> m_Values[i];
   delete [] p -> m_Values;
   delete p;

 }                                                             
//-------------------------------------------------------------------------------------------------
const TCrimeProblem * crimeGeneratorFunc                   ( void )
 {
   printf ( "crimeGen\n" );
   pthread_mutex_lock ( &g_Mtx );
   bool gen = g_Crime-- > 0;
   pthread_mutex_unlock ( &g_Mtx );
   
   if ( ! gen ) return NULL;
   
   TCrimeProblem * p = new TCrimeProblem;
   
   int size = (int)( 30 + 200.0 * rand () / RAND_MAX );
   double ** mat = new double * [size];
   for ( int y = 0; y < size; y ++ )
    {
      mat[y] = new double [size];
      for ( int x = 0; x < size; x ++ )
       mat[y][x] = 1000.0 * rand () / RAND_MAX;
    }
  
   p -> m_Values  = mat;
   p -> m_Size    = size;
   p -> m_MaxCrime= 1000.0 * rand () / RAND_MAX;
   p -> m_Done    = crimeNotify;
   return p;
 }
//-------------------------------------------------------------------------------------------------
void               oneTest                                 ( int               threads,
                                                             int               cost,
                                                             int               crime )
 {
   pthread_mutex_init ( &g_Mtx, NULL );
   g_Cost = cost;
   g_Crime = crime;
   MapAnalyzer ( threads, costGeneratorFunc, crimeGeneratorFunc );
   pthread_mutex_destroy ( &g_Mtx );
 }                                                             
//-------------------------------------------------------------------------------------------------
int                main                                    ( void )
 {
  cout << "----------------------" << endl << "| BASIC TESTS:" << endl << "----------------------" << endl;
   oneTest ( 3, 10, 10 );
   oneTest ( 50, 100, 200 ); 
   cout << "----------------------" << endl << "| ONE THREAD:" << endl << "----------------------" << endl;
   oneTest (1, 10, 10);
   cout << "----------------------" << endl << "| MANY THREADS:" << endl << "----------------------" << endl;
   oneTest (1000, 100, 200);
   return 0;
 } 
