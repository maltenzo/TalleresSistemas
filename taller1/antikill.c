#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	int status;
	pid_t child;

	if (argc <= 1) {
		fprintf(stderr, "Uso: %s comando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		/* S'olo se ejecuta en el Hijo */
		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);
	} else {		
		/* S'olo se ejecuta en el Padre */
		long orig_rax = ptrace(PTRACE_PEEKUSER, child, 8*ORIG_RAX, NULL);
		while(1) {
			if (wait(&status) < 0) { perror("waitpid"); break; }
			int syscall = ptrace(PTRACE_PEEKUSER, child, 8*ORIG_RAX, NULL);
			if (syscall == SYS_kill){
					printf("%s", "hola");
			}else{
				ptrace(PTRACE_CONT,child, NULL, NULL); 
			}
			if (WIFEXITED(status)) break; /* Proceso terminado */
		}
	}
	return 0;
}
