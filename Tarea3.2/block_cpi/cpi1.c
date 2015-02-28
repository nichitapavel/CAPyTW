#include "mpi.h"
#include <stdio.h>
#include <math.h>

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
    int source, dest, tag = 100;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);
    fprintf(stdout,"\nProcess %d of %d on %s\n", myid, numprocs, processor_name);
        n = 100;
        h   = 1.0 / (double) n;
        sum = 0.0;
        for (i = myid + 1; i <= n; i += numprocs) {
            x = h * ((double)i - 0.5);
            sum += f(x);
            printf("aquí myid: %d i: %d\n", myid, i);
        }
        mypi = h * sum;
	printf("\nProcessor: %d, mypi: %.16f\n", myid, mypi);
	if(myid == 0) {
        	pi = mypi;

		for (i = 1; i < numprocs; i++) {
			source = i;           /* MPI process number range is [0,p-1] */
			MPI_Recv(&mypi, 1, MPI_DOUBLE, source, tag,
			         MPI_COMM_WORLD, &status);
			pi += mypi;
		}
                endwtime = MPI_Wtime();
                printf("\npi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
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

            
