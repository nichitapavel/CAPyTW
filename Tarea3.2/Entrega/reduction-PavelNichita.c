/**
 * Computación de Altas Prestaciónes y Tecnnologías Web
 * blockcpi-PavelNichita.c
 * Purpose: Reducción mediante la operación suma utilizando operaciones de 
 *  comunicación punto a punto y sin utilizar las funciones colectivas de MPI
 * 
 * El proceso maestro envia a los procesos esclavos un modulo
 * Los procesos esclavos calcula la suma de 100 numeros aleatorios dentro 
 *  del modulo recibido
 * Los procesos esclavos envian al proceso maestro la suma calculadas
 * El proceso maestro hace la media de todas las sumas recibidas
 * 
 * @author Pavel Nichita
 * @version 1.0 17/02/15
*/

#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

void print_time(void);

int main(int argc, char *argv[]) {
	int  myid, numprocs, i;
	int  namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int source, dest, tag = 100;
	MPI_Status status;
	
/**
 * Descripción de variables 
 * 
 * @mod - todos los numeros sumados estan en el rango 0-19 ó multiplo de 20 (2*20: 0-39 etc.)
 * @sum - suma de 100 números aleatorios dentro del modulo suministrado @mod.
 *  Se calcula en los procesos esclavos
 * @sum_total - Suma de todos las sumas calculadas en los procesos esclavos.
 *  Se calcula en el proceso maestro
*/
	int mod = 20, sum, sum_total;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Get_processor_name(processor_name,&namelen);

	/**
	 * Ejecución del proceso maestro
	 * 
	 * Parte 1
	 * Calcula el modulo de cada proceso esclavos y envia un mensaje a cada uno de ellos
	 * 
	 * Parte 2
	 * Recibe la respuesta de cada proceso esclavo, la suma en un total,
	 *  muestra la suma y la media
	*/
	if (myid == 0) {
		print_time();
		//Envio de mensaje a procesos esclavos
		for (i = 1; i < numprocs; i++){
			dest = i;
			mod *= i; //calculo del modulo de cada proceso
			MPI_Send(&mod, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
		}
		
		sum_total = 0;
		// Recibe la respuesta de cada proceso esclavo y muestra la suma recibida
		for (i = 1; i < numprocs; i++) {
			source = i;
			MPI_Recv(&sum, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			fprintf(stdout,"\nMaster 0 received from process %d number: %d\n", source, sum);
			sum_total += sum; // Acumula la suma de todos los mensajes
		}
		
		// Muestra la suma total (@total_sum) y la media entre numero de procesos esclavos
		fprintf(stdout,"\n\033[22;32mTotal sum: %d Average: %.2f\n\e[m", sum_total,
						(double) sum_total / (numprocs - 1));
	}
	
	/**
	 * Ejecución de un proceso esclavo
	 * 
	 * Parte 1
	 * Recibe el modulo del proceso maestro y calcula la suma de 100
	 *  numeros aleatorios dentro del modulo
	 * 
	 * Parte 2
	 * Envia la suma al proceso maestro
	*/
	else {
		srand(time(NULL));
		
		// Recibe el modulo para calcular la suma de 100 numeros aleatorios
		MPI_Recv(&mod, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		sum = 0;
		
		// Calcula la suma de 100 numeros aleatorios dentro del modulo (rango) suministrado
		for (i = 1; i < 100; i++) {
			sum += rand() % mod;
		}
		
		// Imprime la suma y la envia al proceso maestro
		fprintf(stdout,"\nProcess %d of %d on %s send number: %d\n", myid, numprocs,
						processor_name, sum);
		MPI_Send(&sum, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}


	MPI_Finalize();
	return 0;
}

void print_time(void) {
	char buff[100];
	time_t now = time (0);
	strftime (buff, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
	printf ("\033[22;32mTIME: %s\n\e[m", buff);
}
