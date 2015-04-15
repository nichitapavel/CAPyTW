// OpenMP -- hola.c 
#include <stdio.h>
#include <omp.h>
int main ()  {

    int nthreads, tid;
#pragma omp parallel private(tid)     // Expande un grupo de hilos. 
              // Cada hilo contiene sus propias copias de variables
{
    tid = omp_get_thread_num();       // Obtiene el identificador del hilo
    nthreads = omp_get_num_threads(); // Obtiene el numero de hilos
    printf("Hola desde el hilo %d de %d hilos\n", tid, nthreads);
}  // Todos los hilos se unen al hilo maestro y finalizan

	
}
