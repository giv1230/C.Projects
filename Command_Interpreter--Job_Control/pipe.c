//  * Compile with "gcc pipe.c -o pipe"


#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

// one process sends to another a "hello word" string 

int
main(void)
{
	int		n, fd[2];
	pid_t	pid;
	char	line[2048];

	if (pipe(fd) < 0)
		fprintf(stderr,"pipe error");

	if ( (pid = fork()) < 0)
		fprintf(stderr,"fork error");

	else if (pid > 0) {		/* parent */
		close(fd[0]);
		write(fd[1], "hello world\n", 12);

	} else {				/* child */
		close(fd[1]);
		n = read(fd[0], line, 2048);
		write(1, line, n);
	}

	exit(0);
}
