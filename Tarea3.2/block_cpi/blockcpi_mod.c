/**
 * Computación de Altas Prestaciónes y Tecnnologías Web
 * blockcpi.c
 * Purpose: Calcular PI utilizando paralelismo en bloques
 * 
 * @author Pavel Nichita
 * @version 2.0 16/02/15
*/

#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

void print_time(void);

/**
 * Calcula la fracción de pi para un bloque desde @inf_limit hasta @sup_limit
 * 
 * @param inf_limit limite inferior del bloque a calcular
 * @param sup_limit limite superior del bloque a calcular
 * @param h         constante calculada antes y necesaria para el calculo
 * 
 * @return          la fraccion de pi calculada para el bloque suministrado
*/
double calculate_block(int, int, double);

double f(a)
double a;
{
	return (4.0 / (1.0 + a*a));
}

int main(int argc, char *argv[]) {
	int  n, myid, numprocs, i;
	double PI25DT = 3.141592653589793238462643;
	double mypi, pi, h, sum, x;
	double startwtime, endwtime;
	int  namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int source, dest, i_int, tag = 100;
	MPI_Status status;
	
/**
 * Descripción de variables 
 * 
 * @inf_limit - limite inferior del bloque a calcular una fracción de pi
 * @sup_limit - limite superior del bloque a calcular una fracción de pi
 * @mod_block - modulo de numero de segmentos (n) entre numero de procesadores (numprocs)
 *  si es distinto a "0" los bloques tienen dos tamaños, X y X+1
 * @block_size_1 - tamaño principal de un bloque a calcular una parte de pi, se utiliza siempre
 * @block_size_2 - tamaño segundario de un bloque a calcular una parte de pi, se utiliza 
 *  cuando @mod_block distinto a 0 y no se puede dividir perfectamente los bloques entre
 *  numero de procesadores
*/
	int inf_limit, sup_limit, mod_block, block_size_1, block_size_2;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Get_processor_name(processor_name,&namelen);
	fprintf(stdout,"\nProcess %d of %d on %s\n", myid, numprocs, processor_name);

		n = 39453;
		h = 1.0 / (double) n;
		mod_block  = n % numprocs;
		block_size_1 = n / numprocs;
		block_size_2 = block_size_1;

		/**
		 * si @mod_block es distinto a "0"
		 * el tamaño del bloque principal se incrementa en una unidad
		 * es necesario para los casos cuando no se puede dividir perfectamente
		 * los bloques entre numero de procesadores
		*/
		if (mod_block != 0)
			block_size_1++;

		/**
		 * Control del procesador y el tamaño del bloque a calcular
		 * Ejemplo: 100 secciones entre 3 procesadores
		 * procesador 0 = 34 secciones, procesador 1 = 33 secciones, procesador 2 = 33 secciones
		 * @mod_block = 1;
		 * @myid = 0 (if)
		 * procesador 0 calcula la fracción de pi utilizando 34 secciones
		 * @myid = 1 (else)
		 * procesador 1 calcula la fracción de pi utilizando 33 secciones
		*/
		if (myid < mod_block) {
			// calcular limite inferior y superior del bloque a calcular la fracción de pi
			inf_limit = myid * block_size_1 + 1;
			sup_limit = inf_limit + block_size_1 -1;
			// llamada a la función que calcula la fracción de pi
			mypi = calculate_block(inf_limit, sup_limit, h);
		}
		// solo se utiliza cuando no se puede dividir perfectamente los bloques 
		// entre numero de procesadores
		else {
			// calcular limite inferior y superior del bloque a calcular la fracción de pi
			inf_limit = mod_block * block_size_1 + (myid - mod_block) * block_size_2 + 1;
			sup_limit = inf_limit + block_size_2 -1;
			// llamada a la función que calcula la fracción de pi
			mypi = calculate_block(inf_limit, sup_limit, h);
		}

		printf("\nProcessor: %d, mypi: %.16f\n", myid, mypi);
		if(myid == 0) {
			print_time();
			pi = mypi;

			for (i = 1; i < numprocs; i++) {
				source = i;           /* MPI process number range is [0,p-1] */
				MPI_Recv(&mypi, 1, MPI_DOUBLE, source, tag,
				 MPI_COMM_WORLD, &status);
				pi += mypi;
			}
			endwtime = MPI_Wtime();
			printf("\n\033[22;32mpi is approximately %.16f, Error is %.16f\n\e[m", pi, fabs(pi - PI25DT));
			printf("\nwall clock time = %f\n", endwtime-startwtime);
		}
		else  {
			dest = 0;
			/* send my_result to intended dest.  */
			MPI_Send(&mypi, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
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

// Calcula la fracción de pi para un bloque
double calculate_block(int inf_limit, int sup_limit, double h) {
	double mypi, x, sum = 0.0;

	for (inf_limit; inf_limit <= sup_limit; inf_limit += 1) {
		x = h * ((double)inf_limit - 0.5);
		sum += f(x);
		//printf("aquí myid: %d i_int: %d i: %d\n", myid, i_int, i);
	}
	mypi = h * sum;

	return mypi;
}