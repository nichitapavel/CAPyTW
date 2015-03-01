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
 * *****************************
 * -----**** ATENCIÓN *****-----
 * *****************************
 * Por la linea de comandos se pasan varios argumentos (no hay valores por defecto): 
 * mpirun -n 4 ./matrixSPMDPavelNichita.out 500 40 2 0
 * donde 500 - es el tamaño de la matriz: 500x500
 * donde 40  - es el modulo de matriz A (mirar @module_A)
 * donde 2   - es el modulo de matriz B (mirar @module_B)
 * donde 0   - es si se imprimem las matrices:
 *              valor "0"            - no se imprime
 *              valor distinto a "0" - se imprime
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

/**
 * mult_matrix - Multiplica dos matrices: A y B
 * 
 * @param *matrix_A - Matriz A
 * @param *matrix_B - Matriz B
 * @param rows_A    - Numero de filas matriz A
 * @param rows_B    - Numero de filas matriz B
 * @param size_cols - Numero de columnas, matriz cuadrada = mismo valor matriz A y B
 *
 * @return *        - Multiplicación de matriz A con matriz B
*/
int* mult_matrix(int* matrix_A, int* matrix_B, int rows_A, int rows_B, int size_cols);

/**
 * print_matrix - Imprime una matriz
 * 
 * @param *matrix   - Matriz a imprimir
 * @param size_rows - Numero de filas
 * @param size_cols - Numero de columnas
 * @param myid      - Proceso de donde se imprime
 * @param *str      - Mensaje que se imprime, ej: "Matriz A" 
*/
void print_matrix(int* matrix, int size_rows, int size_cols, int myid, char* str);

/**
 * fill_matrix - Inserta valores en una matriz
 * 
 * @param *matrix   - Matriz a rellanar
 * @param size_rows - Numero de filas
 * @param size_cols - Numero de columnas
 * @param mod       - Modulo de los numeros a rellenar (mirar @module_A o @module_B)
*/
void fill_matrix(int* matrix, int size_rows, int size_cols, int mod);

/**
 * print_array - Imprime un array
 * 
 * @param *array - Array a imprimir
 * @param size   - Tamaño del array
 * @param myid   - Proceso de donde se imprime
 * @param *str   - Mensaje que se imprime, ej: "Array sendcount" 
*/
void print_array(int* array, int size, int myid, char* str);

int main(int argc, char *argv[]) {
		/**
	 *  Descripción de variables - Variables generales;
	 * @root     - proceso principal, en este caso 0
	 * @myid     - identificador proceso "actual"
	 * @numprocs - numero de procesos
	 
	 *  Descripción de variables - Tamaños de matrices
	 * @size_rows   - numero de filas de las matrices, se establece mediante argv[1]
	 * @size_cols   - numero de columnas de las matrices, se establece mediante argv[1]
	 * @matrix_size - numero de elementos de la matriz: @size_rows * @size_cols
	 * @size_rows_recv_matrix_A - numero de filas del trozo enviado de la matriz A
	 
	 *  Descripción de variables - Matrices
	 * @*matrix_A      - Matriz A, multiplicando de la operación, esta matriz se parte 
	 *                   en varios trozos y se envie a los procesos mediante MPI_Scatterv
	 * @*matrix_B      - Matriz B, multiplicador de la operación, se envia entera a los
	 *                   procesos mediante MPI_Bcast
	 * @*matrix_C      - Matriz C, matriz donde se guarda el resultado de la multiplicación
	 *                   entre A y B, los datos se recolentan el ella mediante MPI_Gatherv
	 * @*recv_matrix_A - Matriz con doble funcionalidad, 1. los trozos de la matriz A que se
	 *                  envien con MPI_Scatterv se guarda en ella, 2. la multiplicación entre
	 *                   trozo matriz A y matriz B también se guarda en ella (se sobreescibe)

	 *  Descripción de variables - variables de apoyo
	 * @*sendcount - array donde se guarda el tamaño del trozo de la matriz A, trozo que se
	 *               envia con MPI_Scatterv
	 * @*displs    - array donde se guarda la posicion de la matriz A de donde se empiezan
	 *               a enviar el trozo (displacement)
	 * @mod_block  - variable que nos indica si la matriz se trozea perfectamente al numero
	 *               de procesadores
	 * @index      - variable que se utiliza para calcular la posición donde se empieza a enviar 
	 *               el trozo de la matriz A (@*displs)
	 * @module_A   - modulo de la matriz A, ej: @module_A = 40, matriz con numeros entre 0 y 40
	 *               se recibe mediante argv[2]
	 * @module_B   - modulo de la matriz B, igual que @module_A
	 *               se recibe mediante argv[3]
	 * @print      - imprimir matrices? 0: no, !0 = si, se recibe mediante argv[4]
	 */
	int root = 0, myid, numprocs;
	
	int size_rows, size_cols, matrix_size;
	int size_rows_recv_matrix_A;

	int *matrix_B, *matrix_A, *matrix_C;
	int *recv_matrix_A;

	int *sendcount, *displs;
	int i, mod_block, index = 0;
	int module_A, module_B, print;

	MPI_Status status;

	//asignamos los tamaños de filas y columnas, y tamaño matriz
	size_rows = atoi(argv[1]);
	size_cols = atoi(argv[1]);
	matrix_size = size_rows * size_cols;

	//asignamos los modulos matriz A y matriz B, y si se imprime
	module_A = atoi(argv[2]);
	module_B = atoi(argv[3]);
	print    = atoi(argv[4]);

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	clock_t start = clock();

	/**
	 * Paso 1: Creamos matriz B, multiplicador
	 * y la enviamos por broadcast a todos los procesadores
	 */ 
	matrix_B = malloc(matrix_size*INT);
	if (myid == root) {
		print_time();
		fill_matrix(matrix_B, size_rows, size_cols, module_B);
		MPI_Bcast(matrix_B, matrix_size, MPI_INT, root, MPI_COMM_WORLD);
	}

	if (myid != root) {
		MPI_Bcast(matrix_B, matrix_size, MPI_INT, root, MPI_COMM_WORLD);
	}

	/**
	* Paso 2: Calculamos los tamaños de los trozos de la matriz A
	* que se enviaran a los procesadores y su posición
	* El tamaño se guarda en @*sendcount
	* La posición se guarda en @*displs
	* 
	* si @mod_block es distinto a "0"
	* el los trozos de matriz A no tienen tamaño exacto
	*/
	sendcount = malloc(numprocs*INT);
	displs = malloc(numprocs*INT);
	mod_block  = size_rows % numprocs;
	
	for (i = 0; i < numprocs; i++) {
		if (mod_block > 0) {
			//aumentamos el tamaño del trozo enviar y restamos los numeros de trozos más grandes
			sendcount[i] = (size_rows/numprocs + 1) * size_rows;
			mod_block--;
		}
		else {
			sendcount[i] = size_rows/numprocs * size_rows;
		}
		//calculamos la posicón del trozo en Matriz A
		//ej: tamaño trozo 1 = 4, posición = 0
		//    tamaño trozo 2 = 4, posición = 4
		displs[i] = index;
		index += sendcount[i];
	}

	/**
	 * Paso 3: Reservamos espacio para recibir los trozos de la matriz A
	 */
	recv_matrix_A = malloc(sendcount[myid]*INT);
	size_rows_recv_matrix_A = sendcount[myid]/size_cols; //numero de filas del trozo

	/**
	 * Paso 4: Creamos la matriz A y la enviamos por trozos a las procesos
	 * Una vez enviada eliberamos la memoria
	 */ 
	matrix_A = malloc(matrix_size*INT);
	fill_matrix(matrix_A, size_rows, size_cols, module_A);
	MPI_Scatterv(matrix_A, sendcount, displs, MPI_INT, recv_matrix_A, sendcount[myid], MPI_INT, root, MPI_COMM_WORLD);
	if (print != 0 && myid == root) // Imprimir Matriz A
		print_matrix(matrix_A, size_rows, size_cols, myid, "Matriz A");
	free(matrix_A);

	/**
	 * Paso 5: Hacemos la multiplicacion entre
	 * trozo matriz A y toda la matriz B 
	 */ 
	recv_matrix_A = mult_matrix(recv_matrix_A, matrix_B, size_rows_recv_matrix_A, size_rows, size_cols);

	/**
	 * Paso 6: Recolectamos la los trozos resultantes de la multiplicacion
	 */
	matrix_C = malloc(matrix_size*INT);
	MPI_Gatherv(recv_matrix_A, sendcount[myid], MPI_INT, matrix_C, sendcount, displs, MPI_INT, root, MPI_COMM_WORLD);

	/*
	 * Imprimir Matriz B y Matriz resultante de la multiplicación
	 * */
	if (print != 0 && myid == root) {
		print_matrix(matrix_B, size_rows, size_cols, root, "Matriz B");
		print_matrix(matrix_C, size_rows, size_cols, root, "Matriz Final");
	}
	
	/**
	 * Paso 7: Eliberamos toda la memoria 
	 */
	free(matrix_B);
	free(matrix_C);
	free(sendcount);
	free(displs);
	free(recv_matrix_A);
	
	fprintf(stdout, "Finish Process %d\n", myid);
	clock_t end = clock();

	if(myid == root) {
		fprintf(stdout, "Tamano matriz: %d Procesadores: %d ", size_rows, numprocs);
		fprintf(stdout, "Tiempo de ejecucion: %f\n", (double)(end-start)/CLOCKS_PER_SEC);
	}

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

int* mult_matrix(int* matrix_A, int* matrix_B, int rows_A, int rows_B, int size_cols) {
	int i, j, k;
	int* matrix_C = malloc(rows_A*size_cols*INT);
	
	for (i = 0; i < rows_A; i++) {
		for (j = 0; j < rows_B; j++) {
			matrix_C[i*size_cols+j] = 0;
			for (k = 0; k < rows_B; k++)
				matrix_C[i*size_cols+j] += matrix_A[i*size_cols+k] * matrix_B[k*size_cols+j];
		}
	}

	return matrix_C;
}
