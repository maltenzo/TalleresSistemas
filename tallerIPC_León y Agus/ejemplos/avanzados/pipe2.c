#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MSG "Hola, pipes!"

char *argv0;

int main(int argc, char **argv){
	char rdbuf[128];
	pid_t cpid, mipid;
	int fildes[2];

	argv0 = *argv; argv++; argc--;

	mipid = getpid();
	if (pipe(fildes) == -1) {
		perror("pipe");
		exit(1);
	}

	fprintf(stderr, "[%d] haciendo un fork...\n", (int)mipid);
	cpid = fork();
	if (cpid == -1) {
		perror("fork");
		exit(1);
	}
	if (cpid == 0) {
		/* soy el hijo */
		mipid = getpid();
		close(fildes[1]);
		fprintf(stderr, "[%d] leyendo del fd %d...\n", (int)mipid, fildes[0]);
		if (read(fildes[0], rdbuf, sizeof(MSG)) != sizeof(MSG)) {
			fprintf(stderr, "read[%d]", fildes[0]);
			perror("");
			exit(1);
		}
		close(fildes[0]);
		rdbuf[sizeof(MSG)] = '\0';
		printf("[%d] recibí el mensaje \"%s\"\n", (int)mipid, rdbuf);
	} else {
		/* soy el padre */
		close(fildes[0]);
		fprintf(stderr, "[%d] escribiendo al fd %d...\n", (int)mipid, fildes[1]);
		if (write(fildes[1], MSG, sizeof(MSG)) != sizeof(MSG)) {
			fprintf(stderr, "write[%d]", fildes[1]);
			perror("");
			exit(1);
		}
		close(fildes[1]);
	}

	return 0;
}
