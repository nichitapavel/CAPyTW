#include "mpi.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {
    int myid, numprocs;
    double startwtime, endwtime, prodtime;
    int  namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    double x, y, z;
    long int i, iterations;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);
    fprintf(stdout,"Process %d of %d on %s\n", myid, numprocs, processor_name);
    y = z = 1;
    iterations = 1000000000;
    startwtime = MPI_Wtime();
       for (i = 0; i < iterations; i++) 
	    x = y * z + x;
    endwtime = MPI_Wtime();
    prodtime = (endwtime-startwtime) / (double) iterations;
    printf("wall clock time = %f, Prod time: %.16f, x = %f\n", (endwtime-startwtime), prodtime, x);	       
    MPI_Finalize();
    return 0;
}

            
