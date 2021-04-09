#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sched.h>


int i = 0;

void signal23Handler (int sig){
	write(1,"ya va\n", 6);
	i++;
}

void signal2Handler (int sig) {
	exit(0);
}



int main(int argc, char* argv[]) {
  	// Completar
	pid_t pid = fork();
	if (pid == 0)
	{	
		signal(23, signal23Handler);
		while(i<5){
		}
		pid_t padre = getppid();
		kill(padre,2);
		execvp(argv[1], argv + 1);
		exit(0);
	}
	else
	{
		signal(2, signal2Handler);
		while(i<5){
			write(1, "sup\n", 4);
			kill(pid, 23);
			sleep(1);
			i++;
		}
		pid_t hola = getpid();
		int status;
		wait(&status);
	}


	return 0;
}
