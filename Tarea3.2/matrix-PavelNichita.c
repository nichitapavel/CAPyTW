/**
 * Computación de Altas Prestaciónes y Tecnnologías Web
 * matrix-PavelNichita.c
 * Purpose: distribución y recolección de datos de una matriz utilizando
 *  Scatter y Gather de MPI
 * 
 * @author Pavel Nichita
 * @version 1.0 18/02/15
*/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int** create_matrix(int numprocs);
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

	int** matrix = NULL;
	if (myid == 0) {
		system("clear");
		matrix = create_matrix(numprocs);
	}

	srand(time(NULL));

	int* myid_matrix = (int*) malloc(numprocs * sizeof(int*));

	MPI_Scatter(&matrix, numprocs, MPI_INT,
							myid_matrix, numprocs, MPI_INT,
						 0, MPI_COMM_WORLD);

	for (i = 0; i < numprocs; i++) {
		myid_matrix[i] = rand() % numprocs;
	}

	fprintf(stdout,"\nProcess %d of %d on %s\n", myid, numprocs, processor_name);
	if (myid == 1) {
		for (i = 0; i < numprocs; i++)
			fprintf(stdout, "%d ", myid_matrix[i]);
		fprintf(stdout, "\n");
	}

	int** matrix_2 = create_matrix(numprocs);
	
	MPI_Gather(myid_matrix, numprocs, MPI_INT, 
						 &matrix_2, numprocs, MPI_INT, 
						0, MPI_COMM_WORLD);

						
	if (myid == 0) {
		for (i = 0; i < numprocs; i++) {
			for (j = 0; j < numprocs; j++)
				fprintf(stdout, "%d ", matrix_2[i][j]);
			fprintf(stdout, "\n");
		}

		free(matrix);
	}

	free (matrix_2);
	free(myid_matrix);
	
	MPI_Finalize();
	return 0;
}

void print_time(void) {
	char buff[100];
	time_t now = time (0);
	strftime (buff, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
	printf ("\033[22;32mTIME: %s\n\e[m", buff);
}

int** create_matrix(int numprocs) {
	int i;
	int** matrix = NULL;
	matrix = (int**) malloc(numprocs * sizeof(int*));

	for (i = 0; i < numprocs; i++)
		matrix[i] = (int*) malloc(numprocs * sizeof(int));

	return matrix;
}