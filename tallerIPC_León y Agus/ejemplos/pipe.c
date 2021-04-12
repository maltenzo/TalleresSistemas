#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MSG "Hola, pipes!"

int main(int argc, char **argv){
	char rdbuf[128];
	int fildes[2];
	//Creo el pipe
	if (pipe(fildes) == -1) {
		perror("pipe");
		exit(1);
	}

	fprintf(stderr, "Escribiendo al fd %d...\n", fildes[1]);
	//Escribo en el pipe
	if (write(fildes[1], MSG, sizeof(MSG)) != sizeof(MSG)) {
		fprintf(stderr, "write[%d]", fildes[1]);
		fflush();
		perror("");
		exit(1);
	}

	fprintf(stderr, "Leyendo del fd %d...\n", fildes[0]);
	//Leo en el pipe
	if (read(fildes[0], rdbuf, sizeof(MSG)) != sizeof(MSG)) {
		fprintf(stderr, "read[%d]", fildes[0]);
		perror("");
		exit(1);
	}

	//Imprimo lo que recibí
	rdbuf[sizeof(MSG)] = '\0';
	printf("Recibí el mensaje \"%s\"\n",rdbuf);
	fflush();

	//Cierro los dos extremos del pipe
	close(fildes[1]);
	close(fildes[0]);

	return 0;
}
