/**
 * Computación de Altas Prestaciónes y Tecnnologías Web
 * matrixSPMD-PavelNichita.c
 * Purpose: multiplicación paralela de matrices utilizando
 *  Scatterv y Gatherv de MPI
 * 
 * Se declaran dos matrices @matrix_A y @matrix_B.
 * La matriz @matrix_A se enviá por trozos a los procesos mediante MPI_Scatterv.
 * La matriz @matrix_B se envia a todos los procesos mediante MPI_Bcast.
 * 
 * Se declara la matriz @matrix_C donde se guara el resultado de la multiplicación.
 * En la matriz @matrix_C se colecta las multiplicaciones realizados por los
 *  procesos mediante MPI_Gatherv.
 * 
 * El tamaño de las matrices se pasa por la linea de comandos, por ejemplo: 
 * mpirun -n 4 ./matrixSPMDPavelNichita.out 500
 * donde 500 es el tamaño de la matriz: 500x500
 * 
 * @author Pavel Nichita
 * @version 1.0 28/02/15
*/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Definición de el tamaño de un dato tipo entero (se ahora espacio en el codigo)
#define INT sizeof(int)

void print_time(void);
int* mult_matrix(int* matrix_A, int* matrix_B, int size_rows_A, int size_rows_B, int size_cols);
void print_matrix(int* matrix, int size_rows, int size_cols, int myid, char* str);
void fill_matrix(int* matrix, int size_rows, int size_cols, int mod);
void print_array(int* array, int size, int myid, char* str);

int main(int argc, char *argv[]) {
	int index = 0, root = 0, tag = 100;
	int size_rows, size_cols, matrix_size;
	int i, myid, numprocs, mod_block;
	int *matrix_B, *matrix_A, *matrix_C;
	int *sendcount, *displs;
	MPI_Status status;
	
	size_rows = atoi(argv[1]);
	size_cols = atoi(argv[1]);
	matrix_size = size_rows * size_cols;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

	/**
	 *  Descripción de variables
	 * 
	 * @matrix      - matriz "suprema", donde se guarda los trozos recolectados de los procesadores
	 *  tamaño numero procesadores * numero de procesadores
	 * @myid_matrix - matriz parcial para cada procesador
	*/

	matrix_B = malloc(matrix_size*INT);
	sendcount = malloc(numprocs*INT);
	displs = malloc(numprocs*INT);

	/**
	 * Paso 1: Creamos matriz B, con la cual se multiplicara
	 * y la enviamos por broadcast a todos los procesadores
	 */ 
	if (myid == root) {
		fill_matrix(matrix_B, size_rows, size_cols, 10);
		MPI_Bcast(matrix_B, matrix_size, MPI_INT, root, MPI_COMM_WORLD);
	}

	if (myid != root) {
		MPI_Bcast(matrix_B, matrix_size, MPI_INT, root, MPI_COMM_WORLD);
	}

	/**
	* Paso 2: Calculamos los tamaños de los trozos de la matriz A
	* que se enviaran a los procesadores 
	*/ 

	/**
	* si @mod_block es distinto a "0"
	* el tamaño del bloque principal se incrementa en una unidad
	* es necesario para los casos cuando no se puede dividir perfectamente
	* los bloques entre numero de procesadores
	*/
	mod_block  = size_rows % numprocs;
	for (i = 0; i < numprocs; i++) {
		if (mod_block > 0) {
			sendcount[i] = (size_rows/numprocs + 1)*size_rows;
			mod_block--;
		}
		else {
			sendcount[i] = size_rows/numprocs * size_rows;
		}

		displs[i] = index;
		index += sendcount[i];
	}

	/**
	 * Paso 3: Reservamos espacio para recibir los trozos de la matriz A
	 */
	int* recv_matrix_A = malloc(sendcount[myid]*INT);
	int recv_matrix_A_size = sendcount[myid]*INT;
	int rows = recv_matrix_A_size/(INT*size_cols);

	/**
	 * Paso 4: Creamos la matriz A y la enviamos por trozos a las procesos
	 * Una vez enviada eliberamos la memoria
	 */ 
	matrix_A = malloc(matrix_size*INT);
	fill_matrix(matrix_A, size_rows, size_cols, 2);
	if (myid == root) // Imprimir Matriz A
		print_matrix(matrix_A, size_rows, size_cols, myid, "Matriz A");
	MPI_Scatterv(matrix_A, sendcount, displs, MPI_INT, recv_matrix_A, recv_matrix_A_size, MPI_INT, root, MPI_COMM_WORLD);
	free(matrix_A);

	/**
	 * Paso 5: Hacemos la multiplicacion entre
	 * trozo matriz B y toda la matriz A 
	 */ 
	matrix_C = mult_matrix(recv_matrix_A, matrix_B, rows, size_rows, size_cols);

	/**
	 * Paso 6: Recolectamos la los trozos resultantes de la multiplicacion
	 */
	int* matrix_final = malloc(matrix_size*INT);
	MPI_Gatherv(matrix_C, sendcount[myid], MPI_INT, matrix_final, sendcount, displs, MPI_INT, root, MPI_COMM_WORLD);

	/*
	 * Imprimir Matriz B y Matriz resultante de la multiplicación
	 * */
	if (myid == root) {
		print_matrix(matrix_B, size_rows, size_cols, root, "Matriz B");
		print_matrix(matrix_final, size_rows, size_cols, root, "Matriz Final");
	}
	
	/**
	 * Paso 7: Eliberamos toda la memoria 
	 */
	free(matrix_B);
	free(recv_matrix_A);
	free(matrix_C);
	free(matrix_final);
	
	fprintf(stdout, "Finish Process %d\n", myid);

	MPI_Finalize();
	return 0;
}

void print_time(void) {
	char buff[100];
	time_t now = time (0);
	strftime (buff, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
	fprintf (stdout, "\033[22;32mTIME: %s\n\e[m", buff);
}

void print_matrix(int* matrix, int size_rows, int size_cols, int myid, char* str) {
	int i, j;

	fprintf(stdout, "\033[22;32mProcess nº: %d %s\n\e[m", myid, str);
	for (i = 0; i < size_rows; i++) {
		for (j = 0; j < size_cols; j++)
			fprintf(stdout, "%d ", matrix[i*size_cols+j]);
		fprintf(stdout, "\n");
	}
}

void fill_matrix(int* matrix, int size_rows, int size_cols, int mod) {
	int i, j;

	srand(time(NULL));
	for (i = 0; i < size_rows; i++)
		for (j = 0; j < size_cols; j++)
			matrix[i*size_cols+j] = rand() % mod;	
}

void print_array(int* array, int size, int myid, char* str) {
	int i;
	
	fprintf(stdout, "\033[22;32mProcess: %d %s\n\e[m", myid, str);
	for (i = 0; i < size; i++)
		fprintf(stdout, "%d ", array[i]);
	fprintf(stdout, "\n");
}

int* mult_matrix(int* matrix_A, int* matrix_B, int size_rows_A, int size_rows_B, int size_cols) {
	int i, j, k;
	int* matrix_C = malloc(size_rows_A*size_cols*INT);
	
	for (i = 0; i < size_rows_A; i++) {
		for (j = 0; j < size_rows_B; j++) {
			matrix_C[i*size_cols+j] = 0;
			for (k = 0; k < size_rows_B; k++)
				matrix_C[i*size_cols+j] += matrix_A[i*size_cols+k] * matrix_B[k*size_cols+j];
		}
	}

	return matrix_C;
}
