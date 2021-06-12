/*
   File: binsem.c
   Created on: 19/04/18
   Written by: Eli Kessem
   ID:	066127549 
 
   Description: this file implement a simple binary semaphores library for
                user-level threads.

 */

#include "binsem.h"
#include <signal.h> // for SIGALRM
#include <unistd.h> //for getpid

/*  Initializes a binary semaphore  */
void binsem_init(sem_t *s, int init_val)
{
	*s = init_val;
}

/*  The Up() operation  */
void binsem_up(sem_t *s)
{
	xchg(s, 1);
}

/*  The Down() operation  */
int binsem_down(sem_t *s)
{
	while (xchg(s, 0) == 0)
		if (kill(getpid(), SIGALRM) == -1)
			return -1;

	return 0;
}