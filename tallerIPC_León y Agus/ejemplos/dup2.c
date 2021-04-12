#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // me da  O_WRONLY y O_CREAT

int main(int argc, char **argv){
	//Abro el archivo, le digo que escriba y de ser necesario lo cree.
	//Tambien le doy permisos al usuario para usar el archivo.
	int fd = open("archivo.txt", O_WRONLY | O_CREAT,S_IRUSR | S_IWUSR);
	//Copio el fd
	dup2(fd, STDOUT_FILENO);
	//Ejecutamos echo
	char * args[] = {"echo", "Es jueves y mi PCB lo sabe", NULL};
	execvp("echo", args);
	//Esto es innecesario
	//PEEEERO... Recuerden cerrar los fd!
	close(fd);
}
