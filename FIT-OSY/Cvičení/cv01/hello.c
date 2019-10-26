#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 8 

void * helloThread ( void * threadid )
{
  sleep(1);
  printf("Thread %d: Hello World!\n", (int) threadid);
  return(NULL);
}

int main ( int argc, char * argv[] )
{
  pthread_t threads[NUM_THREADS];
  int rc, t;

  for ( t=0; t<NUM_THREADS; t++) {
    printf("Main: Creating thread %d\n", t);

    /*
     * výstupní identifikátor threadu
     *
     * název předávané funkce
     * vstupní parametr předávané funkce
     */
    rc = pthread_create(&threads[t], NULL, helloThread, (void *)t);
    if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      return(EXIT_FAILURE);
    }
  }
  /* toto neni standardni zpusob ukonceni programu s vlakny
   */
  pthread_exit(NULL);
  return ( 0 ); /* ale zde je to zbytecne - nedosazitelne */
}

