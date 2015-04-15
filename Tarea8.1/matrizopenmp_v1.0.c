#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int* mult_matrix(int* matrix_A, int* matrix_B, int rows, int cols);
void fill_matrix(int* matrix, int size_rows, int size_cols, int mod);
void print_matrix(int* matrix, int size_rows, int size_cols, char* str);

int main(int argc, char const *argv[])
{
	int *A, *B, *C;
	int rows, cols, matrix_size;
	int mod_A, mod_B;
	int print, test;
	double t1, t2;
	
	rows = atoi(argv[1]);
	cols = atoi(argv[1]);
	matrix_size = rows * cols;
	
	mod_A = atoi(argv[2]);
	mod_B = atoi(argv[3]);
	print = atoi(argv[4]);
	
	A = malloc(matrix_size * sizeof(int));
	B = malloc(matrix_size * sizeof(int));
	fill_matrix(A, rows, cols, mod_A);
	fill_matrix(B, rows, cols, mod_B);
	
	t1 = omp_get_wtime();
	C = mult_matrix(A, B, rows, cols);
	t2 = omp_get_wtime(); 
	fprintf(stdout, "Tiempo de ejecucion: %f\n", t2 - t1);
	
	if (print != 0) {
		print_matrix(A, rows, cols, "Matriz A");
		print_matrix(B, rows, cols, "Matriz B");
		print_matrix(C, rows, cols, "Matriz C");
	}
	
	free(A);
	free(B);
	free(C);
	
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

#pragma omp parallel shared(matrix_A, matrix_B, matrix_C, rows, cols) private(i, j, k)
{
#pragma omp for
	for (i = 0; i < rows; i++) {
		for (j = 0; j < rows; j++) {
			matrix_C[i*cols+j] = 0;
			for (k = 0; k < rows; k++) {
				matrix_C[i*cols+j] += matrix_A[i*cols+k] * matrix_B[k*cols+j];
			}
		}
	}
}

	return matrix_C;
}