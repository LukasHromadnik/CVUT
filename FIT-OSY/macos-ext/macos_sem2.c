#if __APPLE__
/*
 *  
 *   OS X POSIX semaphores emulation with System V sempathores.
 *   The file maps sem_functions to my_sem_functions.
 *
 *   Version 2.0
 *   Tomas Zahradnicky <zahradt@fit.cvut.cz>
 *
 */
#include <sys/sem.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdlib.h>

#define sem_init 	my_sem_init
#define sem_destroy	my_sem_destroy
#define sem_post	my_sem_post
#define sem_wait	my_sem_wait

int my_sem_destroy( sem_t *sem )
{
        int err;

	/* Sanity check */
	if( sem )
	{
		/* Dispose of the semaphore */
		err = semctl( *sem, 0, IPC_RMID );

		if( err == 0 )
		{
			/* Zero the reference */
			*(int*)sem = 0;
		}
	}
	else
	{
		/* Invalid semaphore */
		err = -1;
		errno = EINVAL;
	}
        
        return err;
        
}

int my_sem_init( sem_t* sem, int a, int b )
{        
        int err;
        struct sembuf sbuf;

	/* Sanity check */       
	if( sem == NULL )
	{
		errno = EINVAL;
		return -1;
	}

	/* Create a new private semaphore */
	*(int*)sem = semget( IPC_PRIVATE, 1, IPC_CREAT | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
	if( *(int*)sem == -1 )
		return -1;

	/* Set the initial value for the semaphore to b */
	sbuf.sem_num = 0;
	sbuf.sem_op = b;
	sbuf.sem_flg = 0;
	err = semop( *(int*)sem, &sbuf, 1 );

	/* If we succeeded, return */
	if( err != 0 )
	{	
		/* Keep errno so we can set it back after destroying the semaphore */
		err = errno;

		/* Destroy the semaphore if we failed */	
		my_sem_destroy( sem );

		/* Reset errno to the original failure */
		errno = err;

		/* Indicate an error */
		err = -1;
	}

	return err;
}

int my_sem_post( sem_t* sem )
{
        int err = 0;
        struct sembuf sop;

	/* Sanity check */       
	if( sem == NULL )
	{
		errno = EINVAL;
		return -1;
	} 

	/* Increase by 1 */
        sop.sem_num = 0;
        sop.sem_op = 1;
        sop.sem_flg = 0;
        
        for(;;)
        {
		/* Post the semaphore */
                err = semop( *(int*)sem, &sop, 1 );
                
		/* If post succeeded, return */
                if( err == 0 )
                        break;
                else if( err == -1 )
                {
			/* Post failed, find out why */
                        err = errno;
                        
                        if( err == EINTR )
                        {
				/* Post got interrupted, must retry! */
			        continue;
                        }
			else
			{
				/* Another failure, return */
                                break;
			}
                }
		else
		{
			/* Should never happen */
			abort();
		}
        }
        
        return err;
}

int my_sem_wait( sem_t* sem )
{
        int err = 0;
        struct sembuf sop;
       
	/* Sanity check */       
	if( sem == NULL )
	{
		errno = EINVAL;
		return -1;
	} 

	/* Decrease by 1 */ 
        sop.sem_num = 0;
        sop.sem_op = -1;
        sop.sem_flg = 0;
        
        for(;;)
        {
		/* Post the semaphore */
                err = semop( *(int*)sem, &sop, 1 );
                
		/* If post succeeded, return */
                if( err == 0 )
                        break;
                else if( err == -1 )
                {
			/* Post failed, find out why */
                        err = errno;
                        
			if( err == EINTR )
                        {
				/* Post got interrupted, must retry! */
			        continue;
                        }
			else
			{
				/* Another failure, return */
                                break;
			}
                }
		else
		{
			/* Should never happen */
			abort();
		}
        }
        
        return err;
}

#endif


