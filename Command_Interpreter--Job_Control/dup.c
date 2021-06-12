/* 
 * The same as pipe.c, just due to the dup function thr ipc communication 
 * between processes becomes transparent (via the stdout and stdin)
 * Compile with "gcc dup.c -o dup"
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

int
main(void)
{
	int n, fd[2];
	pid_t	pid;
	char	line[2048];

	if (pipe(fd) < 0)
		fprintf(stderr,"pipe error");

	if ( (pid = fork()) < 0)
		fprintf(stderr,"fork error");

	else if (pid > 0) {		        /* parent */
		close(fd[0]);      

		/* redirection of stdout so that stdout 
                   of the process would be actualy 
		   written to the pipe 
		*/
                close(1); /* stdout */
		dup(fd[1]); 
		close(fd[1]);
		write(1, "hello world\n", 12);

	} else {				/* child */
		close(fd[1]);

		/* redirection of stdin so that stdin would 
		   be actualy read from pipe 
		*/
		close(0); /*stdin*/
		dup(fd[0]);
		close(fd[0]);
		n = read(0, line, 2048);
		write(1, line, n);
	}

	exit(0);
}
