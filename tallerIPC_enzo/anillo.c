#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int randomMayora(int mensajeInicial){
	int random = 0;
	srand(time(NULL) ^ (getpid()<<16));
	while(random < mensajeInicial){
		random = rand() % 200 ;
	}
	return random;
}



int main(int argc, char const* argv[]){
	//cargo las variables
	int cantidadDeProcesos = (int)strtol(argv[1], NULL, 10);
	int cantidadDePipes = cantidadDeProcesos/2;
	int mensajeInicial = (int) strtol(argv[2], NULL, 10);
	int procesoQueIniciaLaComunicacion = (int) strtol(argv[3], NULL, 10);
	int miPipe = cantidadDePipes-1;
	int miNumeroDeProceso = 0;
	int pipes [cantidadDePipes][2];
	//creo los pipes
	for(int i = 0; i<cantidadDePipes; i++){
		int iesimoPipe[2];
		if(pipe(iesimoPipe) == -1){
			perror("pipe");
			exit(1);
		}else{
			pipes[i][0] = iesimoPipe[0];
			pipes[i][1] = iesimoPipe[1];
		}
	}
	//creo los procesos
	for(int j = 1; j<cantidadDeProcesos; j++){
		pid_t child = fork();
		if (child == 0) {
			miPipe = j/2;
			miNumeroDeProceso = j;
			printf("creaste el proceso numero%d", j);
			break;
		}
	}

	//cierro pipes
	for(int k = 0; k<cantidadDePipes; k++){
		//si no es mi anterior cierro la lectura de ese pipe
		if (k != miPipe) {close(pipes[k][0]);}
		//si no es mi siguiente cierro la escritura en ese pipe
		if (k != ((miPipe+1) % cantidadDePipes)) {close(pipes[k][1]);}
	}

	//si soy el que inicio la comunicacion creo el numero secreto y lo escribo
	int miNumero=0;
	int numeroSecreto=0;
	if (miNumeroDeProceso == procesoQueIniciaLaComunicacion){
//		printf("soy el proceso %d", miNumeroDeProceso);
		numeroSecreto = randomMayora(mensajeInicial);
		miNumero = mensajeInicial+1;
		if(write(pipes[(miPipe+1) % cantidadDePipes][1], &miNumero, sizeof(int)) != sizeof(int)){
			perror("write");
			exit(1);
		}
	}
	int lectura = 0;
	while(1){
		if(read(pipes[miPipe][0], &lectura, sizeof(int)) == sizeof(int) & lectura != miNumero-1){

			if(miNumeroDeProceso == procesoQueIniciaLaComunicacion){
				if(numeroSecreto <= lectura){

					printf("el numero secreto era %d \n", numeroSecreto);
					printf("el ultimo numero fue %d \n", lectura);
					write(pipes[(miPipe+1)%cantidadDePipes][1], 0, sizeof(int));
					close(pipes[miPipe][0]);
					close(pipes[(miPipe+1)%cantidadDePipes][1]);
					printf("cerre el proceso numero %d", miNumeroDeProceso);
					exit(0);

				}else{
					miNumero = lectura + 1;
					write(pipes[(miPipe+1) % cantidadDePipes][1], &miNumero, sizeof(int));
				}
			}else{
				if(lectura == 0){
					write(pipes[(miPipe+1)%cantidadDePipes][1], 0, sizeof(int));
					close(pipes[(miPipe+1)%cantidadDePipes][1]);
					close(pipes[miPipe][0]);
					printf("cerre el proceso numero %d", miNumeroDeProceso);
					exit(0);
				}else{
					miNumero = lectura + 1;
					write(pipes[(miPipe+1)%cantidadDePipes][1], &miNumero, sizeof(int));
				}
			}
			//write(1, &miNumero, sizeof(int))
		}


	}

}
