/**
 * Computación de Altas Prestaciónes y Tecnnologías Web
 * matrix-PavelNichita.c
 * Purpose: distribución y recolección de datos de una matriz utilizando
 *  Scatter y Gather de MPI
 * 
 * Se declara una matriz cuadrada del tamaño de numero de procesadores
 *  que se reparte a cada procesadores, cada uno de ellos rellenan la matriz
 *  segun una formula matematica que toma en cuenta el procesador, numero
 *  total de procesadores y posición actual
 * 
 * @author Pavel Nichita
 * @version 1.0 18/02/15
*/

#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

void print_time(void);

int main(int argc, char *argv[]) {
	int  myid, numprocs, i, j;
	int  namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int source, dest, tag = 100;
	MPI_Status status;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Get_processor_name(processor_name,&namelen);

	/**
	 *  Descripción de variables
	 * 
	 * @matrix      - matriz "suprema", donde se guarda los trozos recolectados de los procesadores
	 *  tamaño numero procesadores * numero de procesadores
	 * @myid_matrix - matriz parcial para cada procesador
	*/
	
	int matrix[numprocs][numprocs];
	int myid_matrix[numprocs];

	// Enviamos los trozos de la matriz suprema a cada procesador
	MPI_Scatter(&matrix, numprocs, MPI_INT,
							&myid_matrix, numprocs, MPI_INT,
						 0, MPI_COMM_WORLD);

	// Rellenamos el trozo de la matriz con datos
	for (i = 0; i < numprocs; i++) {
		myid_matrix[i] = myid * numprocs + i;
	}

	// Mostramos el trozo con los nuevos datos
	fprintf(stdout,"\nProcess %d of %d on %s has myid_matrix:\n", myid, numprocs, processor_name);
	for (i = 0; i < numprocs; i++)
		fprintf(stdout, "%d ", myid_matrix[i]);
	fprintf(stdout, "\n");

	// Recolectamos la matriz por trozos
	MPI_Gather(&myid_matrix, numprocs, MPI_INT, 
						 &matrix, numprocs, MPI_INT, 
						0, MPI_COMM_WORLD);

	// Mostramos la matriz final desde proceso maestro
	if (myid == 0) {
		print_time();
		fprintf(stdout, "Matrix is: \n");
		for (i = 0; i < numprocs; i++) {
			for (j = 0; j < numprocs; j++)
				fprintf(stdout, "%d ", matrix[i][j]);
			fprintf(stdout, "\n");
		}
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
