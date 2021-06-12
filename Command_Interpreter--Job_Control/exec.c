//  * Compile with "gcc exec.c -o exec"

#include	<sys/types.h>
#include	<sys/wait.h>
#include        <stdio.h>
#include        <unistd.h>
#include        <string.h>
#include        <stdlib.h>


// child process is forked. 
// the child executes "ls -al"
// the parent process "waits" the child and collects it

int
main(void)
{

	char *args[3];
	pid_t	pid;

	if ( (pid = fork()) < 0)
		fprintf(stderr,"fork error");
	else if (pid == 0) {			/* child */
		args[0] = strdup("ls");
		args[1] = strdup("-al");
		args[2] = NULL;
		if (execvp(args[0], args)< 0)
		  fprintf(stderr,"execl error");
	}
	
	if (waitpid(pid, NULL, 0) < 0)	/* parent */
		fprintf(stderr,"waitpid error");
	exit(0);
}
