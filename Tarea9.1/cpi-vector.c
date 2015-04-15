/*
   Código que prodice False Sharing
*/

#include "omp.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

#define THREADNUM 4



double f(double a) {
    return (4.0 / (1.0 + a*a));
}

void calculo_pi_vector() {
        double t1, t2;
	int i;
	unsigned long int n;
	double PI25DT = 3.141592653589793238462643;
	double pi = 0.0, h, sum, x;
	double v[THREADNUM];
	for (i = 0; i < THREADNUM; i++) {
		v[i] = 0.0;
	}

	n = 1000000000;
	h   = 1.0 / (double) n;
	sum = 0.0;
        t1 = omp_get_wtime(); 
	#pragma omp parallel private (pi, x, i) shared (v)
	{
		#pragma omp for
        	for (i = 1; i <= n; i ++) {
	            x = h * ((double)i - 0.5);
        	    v[omp_get_thread_num()] += f(x); // Efecto false sharing
	        }
	}
	for (i = 0; i < THREADNUM; i++) {
		pi += v[i];  
	}
        t2 = omp_get_wtime(); 
	pi *= h;
	printf("PI = %.16f; Tiempo: %.16f\n", pi, t2 - t1);
}

int main(int argc, char *argv[]) {
	omp_set_num_threads(THREADNUM);				
	calculo_pi_vector();
	return 0;
}

            
