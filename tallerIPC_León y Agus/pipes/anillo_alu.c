#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <syscall.h>
#include <time.h>


int main(int argc, char **argv)
{	
	int status, n, c, s;
	int id = 0;
	int buffer[1];

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
	
  	/* COMPLETAR */
	
	sscanf (argv[1],"%d",&n);
	sscanf (argv[2],"%d",&c);
	sscanf (argv[3],"%d",&s);

	printf("n = %d, c = %d, s = %d \n", n, c, s);
//	n =(int) argv[0];
//	c =(int) argv[1];
//	s =(int) argv[2];
	pid_t pid_child;
	pid_t pid[n];

	int fdPipe[n+1][2];


	for(int i=0; i<=n; i++){
		if (pipe(fdPipe[i]) == -1) {
			perror("pipe");
			exit(1);
		}
	}


    // fprintf(stderr, "\nSe crearán %d procesos, se enviará el caracter %d desde proceso %d \n", n, buffer[0], start);

	id = 0;
	int i = 1;
	
	while(i <= n){
		//fork
		pid_child = fork();
		if(pid_child == 0){
			id = i;
			//los hijos escriben en el pipe id%n y leen del id-1. El pipe n es del padre.
			/*for(int j=0; j<n; j++){
				if(id%n != j){
					close(fdPipe[j][1]);
				}
				if(id-1 != j){
					close(fdPipe[j][0]);
				}
				
			}
			if(id != s){
					close(fdPipe[n][1]);
					close(fdPipe[n][0]);
				}
			*/
			//exit(0);
			i=n+1;
		}else{		
			pid[i-1] = pid_child;
			//close(fdPipe[i-1][1]);
			//close(fdPipe[i-1][0]);
			i = i+1;
		}
	}


	for(int i = 0; i<n; i++){

		if(id%n != i || id == 0){
			close(fdPipe[i][1]);
		}
		if(id-1 != i || id == 0){
			close(fdPipe[i][0]);
		}
	}

	if(id != s && id != 0){
				close(fdPipe[n][1]);
				close(fdPipe[n][0]);
		}



	//comportamiento
	if(pid_child == 0){
		int resultado;

		if(id == s){
			
			if (read(fdPipe[n][0], buffer, sizeof(int)) <= 0) {
				fprintf(stderr, "read[%d]", fdPipe[n][0]);
				perror("");
				exit(1);
			}

			fprintf(stderr, "\nse leyò %d del padre \n", buffer[0]);

			srand(time(NULL));
			int num_secreto = rand()%10 +1;
			resultado = num_secreto -1;

			fprintf(stderr, "\nnùmero secreto es %d \n", num_secreto);

			while(resultado < num_secreto){
				resultado = buffer[0] + 1;

				if (write(fdPipe[id%n][1], &resultado, sizeof(int)) <= 0) {
					fprintf(stderr, "write[%d]", fdPipe[id%n][1]);
					perror("");
					exit(1);
				}

				fprintf(stderr, "\nse le pasò %d al proceso %d \n", resultado, (id%n)+1);

				if (read(fdPipe[id-1][0], buffer, sizeof(int)) <= 0) {
					fprintf(stderr, "read[%d]", fdPipe[id-1][0]);
					perror("");
					exit(1);
				}	

				resultado = buffer[0];
			}

			if (write(fdPipe[n][1], &resultado, sizeof(int)) <= 0) {
					fprintf(stderr, "write[%d]", fdPipe[n][1]);
					perror("");
					exit(1);
				}

		}else{
			//HAY QUE PONER WHILE + HACER BIEN QUE CUANDO EL READ DA 0 SE VAYA DEL WHILE Y CIERRE
			while(1){
				if (read(fdPipe[id-1][0], buffer, sizeof(int)) <= 0) {
					fprintf(stderr, "read[%d]", fdPipe[id-1][0]);
					perror("");
					break;
					//exit(1);
				}

				fprintf(stderr, "\nyo, %d, recibì %d \n", id, resultado);
				resultado = buffer[0] + 1;

				if (write(fdPipe[id%n][1], &resultado, sizeof(int)) <= 0) {
					fprintf(stderr, "write[%d]", fdPipe[id%n][1]);
					perror("");
					exit(1);
				}

				fprintf(stderr, "\n%d le pasò %d al proceso %d \n", id, resultado, (id%n)+1);
			}
		}
	}else{

		if (write(fdPipe[n][1], &c, sizeof(int)) <= 0) {
			fprintf(stderr, "read[%d]", fdPipe[n][1]);
			perror("");
			exit(1);
		}

		if (read(fdPipe[n][0], buffer, sizeof(int)) <= 0) {
			fprintf(stderr, "read[%d]", fdPipe[n][0]);
			perror("");
			exit(1);
		}

		fprintf(stderr, "\nresultado de las vueltas: %d \n", buffer[0]);
	}



	if(id != 0){
		close(fdPipe[id%n][1]);
		close(fdPipe[id-1][0]);
	}//else{
	//	for(int child = 0; child < sizeof pid ; child++ ){
	//		kill(pid[child], SIGKILL);
	//	}
	//}
	if(id == s || id == 0){
		close(fdPipe[n][1]);
		close(fdPipe[n][0]);
	}

	if(id == 0){
		int* bleh;
		for(int i=0; i<n; i++){
			waitpid(pid[i],  bleh, 0);
		}
	}

	fprintf(stderr, "\nproceso %d, me voy \n", id);
    /* COMPLETAR */
}
