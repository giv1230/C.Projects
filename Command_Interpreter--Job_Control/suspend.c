// this program presents how to block signal SIGINT
// while running in critical region
// * Compile with "gcc suspend.c -o suspend"

#include	<signal.h>
#include        <stdio.h>
#include        <unistd.h>
#include        <stdlib.h>
#include        <stdio.h>

static void	sig_int(int);

int
main(void)
{
  sigset_t	newmask, oldmask, zeromask;
  
  if (signal(SIGINT, sig_int) == SIG_ERR)
    fprintf(stderr,"signal(SIGINT) error");
  
  sigemptyset(&zeromask);
  
  sigemptyset(&newmask);
  sigaddset(&newmask, SIGINT);
  
  /* block SIGINT and save current signal mask */
  if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    fprintf(stderr,"SIG_BLOCK error");
  
  /* critical region of code */
  printf("In critical region: SIGINT will be blocked for 10 sec.\n");
  printf("Type Ctrl-C in first 10 secs and see what happens.\n");
  printf("Then run this program again and type Ctrl-C when 10 secs elapsed.\n");
  fflush(stdout);
  
  
  sleep(10);
  
  /* allow all signals and pause */
  if (sigsuspend(&zeromask) != -1)
    fprintf(stderr,"sigsuspend error");
  
  printf("after return from sigsuspend: ");
  
  /* reset signal mask which unblocks SIGINT */
  if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    fprintf(stderr,"SIG_SETMASK error");
  
  /* and continue processing ... */
  exit(0);
}

static void
sig_int(int signo)
{
  printf("\nIn sig_int: SIGINT\n"); fflush(stdout);
  return;
}
