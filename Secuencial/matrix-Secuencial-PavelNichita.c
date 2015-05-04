/**
 * Computación de Altas Prestaciónes y Tecnnologías Web
 * matrix-Secuencial-PavelNichita.c
 * Purpose: multiplicación de matrices secuencial
 * 
 * Se declaran dos matrices @matrix_A y @matrix_B, las que se multiplican.
 * 
 * Se declara la matriz @matrix_C donde se guara el resultado de la multiplicación.
 * 
 * *****************************
 * -----**** ATENCIÓN *****-----
 * *****************************
 * Por la linea de comandos se pasan varios argumentos (no hay valores por defecto): 
 * ./matrix.out 500 40 2 0
 * donde 500 - es el tamaño de la matriz: 500x500
 * donde 40  - es el modulo de matriz A (mirar @module_A)
 * donde 2   - es el modulo de matriz B (mirar @module_B)
 * donde 0   - es si se imprimem las matrices:
 *              valor "0"            - no se imprime
 *              valor distinto a "0" - se imprime
 * 
 * @author Pavel Nichita
 * @version 1.0 04/05/15
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * mult_matrix - Multiplica dos matrices: A y B
 * 
 * @param *matrix_A - Matriz A
 * @param *matrix_B - Matriz B
 * @param rows - Numero de filas de las matrices
 * @param cols - Numero de columnas de las matrices
 *
 * @return *        - Multiplicación de matriz A con matriz B
*/
int* mult_matrix(int* matrix_A, int* matrix_B, int rows, int cols);

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
 * print_matrix - Imprime una matriz
 * 
 * @param *matrix   - Matriz a imprimir
 * @param size_rows - Numero de filas
 * @param size_cols - Numero de columnas
 * @param *str      - Mensaje que se imprime, ej: "Matriz A" 
*/
void print_matrix(int* matrix, int size_rows, int size_cols, char* str);

int main(int argc, char const *argv[])
{
	/**
	 *  Descripción de variables - Tamaños de matrices
	 * @rows   - numero de filas de las matrices, se establece mediante argv[1]
	 * @cols   - numero de columnas de las matrices, se establece mediante argv[1]
	 * @matrix_size - numero de elementos de la matriz: @size_rows * @size_cols
	 *
	 *  Descripción de variables - Matrices
	 * @*matrix_A      - Matriz A, multiplicando de la operación
	 * @*matrix_B      - Matriz B, multiplicador de la operación
	 * @*matrix_C      - Matriz C, matriz donde se guarda el resultado de la multiplicación
	 *                   entre A y B
	 *
	 *  Descripción de variables - variables de apoyo
	 * @module_A   - modulo de la matriz A, ej: @module_A = 40, matriz con numeros entre 0 y 40
	 *               se recibe mediante argv[2]
	 * @module_B   - modulo de la matriz B, igual que @module_A
	 *               se recibe mediante argv[3]
	 * @print      - imprimir matrices? 0: no, !0 = si, se recibe mediante argv[4]
	 *
	 *  Descripción de variables - variables de tiempo
	 * @t1  - Tiempo cogido antes de empezar la mutliplicación
	 * @t2  - Tiempo cogido después de acabar la mutliplicación
	 */
	int *matrix_A, *matrix_B, *matrix_C;
	int rows, cols, matrix_size;
	int module_A, module_B;
	int print;
	double t1, t2;
	
	//asignamos los tamaños de filas y columnas, y tamaño matriz
	rows = atoi(argv[1]);
	cols = atoi(argv[1]);
	matrix_size = rows * cols;
	
	//asignamos los modulos matriz A y matriz B, y si se imprime
	module_A = atoi(argv[2]);
	module_B = atoi(argv[3]);
	print = atoi(argv[4]);
	
	//Asignar memoria y rellenar con datos las matrices @matrix_A y @matrix_B
	matrix_A = malloc(matrix_size * sizeof(int));
	matrix_B = malloc(matrix_size * sizeof(int));
	fill_matrix(matrix_A, rows, cols, module_A);
	fill_matrix(matrix_B, rows, cols, module_B);
	
	clock_t start = clock();
	matrix_C = mult_matrix(matrix_A, matrix_B, rows, cols);
	clock_t end = clock();
	fprintf(stdout, "Tamaño: %d Tiempo de ejecucion: %f\n", rows, (double)(end-start)/CLOCKS_PER_SEC);
	
	if (print != 0) {
		print_matrix(matrix_A, rows, cols, "Matriz A");
		print_matrix(matrix_B, rows, cols, "Matriz B");
		print_matrix(matrix_C, rows, cols, "Matriz C");
	}
	
	free(matrix_A);
	free(matrix_B);
	free(matrix_C);
	
  return 0;
}

void fill_matrix(int* matrix, int size_rows, int size_cols, int mod){
    int i, j;
    srand(time(NULL));

    for (i = 0; i < size_rows; i++)
        for (j = 0; j < size_cols; j++)
            matrix[i*size_rows+j] = rand() % mod;
}

void print_matrix(int* matrix, int size_rows, int size_cols, char* str) {
    int i, j;

    fprintf(stdout, "\033[22;32m%s\n\e[m", str);
    for (i = 0; i < size_rows; i++) {
        for (j = 0; j < size_cols; j++)
            printf("%d ", matrix[i*size_cols+j]);
        printf("\n");
    }
}

int* mult_matrix(int* matrix_A, int* matrix_B, int rows, int cols) {
	int i, j, k;
	int* matrix_C = malloc(rows * cols * sizeof(int));


	for (i = 0; i < rows; i++) {
		for (j = 0; j < rows; j++) {
			matrix_C[i*cols+j] = 0;
			for (k = 0; k < rows; k++) {
				matrix_C[i*cols+j] += matrix_A[i*cols+k] * matrix_B[k*cols+j];
			}
		}
	}

	return matrix_C;
}