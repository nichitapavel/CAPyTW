#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

void print_time(void);
double f(a)
double a;
{
	return (4.0 / (1.0 + a*a));
}

int main(int argc, char *argv[]) {
	int  n, myid, numprocs, i, block_size;
	double PI25DT = 3.141592653589793238462643;
	double mypi, pi, h, sum, x;
	double startwtime, endwtime;
	int  namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int source, dest, i_int, tag = 100;
	MPI_Status status;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Get_processor_name(processor_name,&namelen);
	fprintf(stdout,"\nProcess %d of %d on %s\n", myid, numprocs, processor_name);
	
		n = 100;
		h   = 1.0 / (double) n;
		sum = 0.0;
		block_size = n/numprocs;
		for (i = 1; i <= block_size; i += 1) {
			i_int = myid * block_size + i;
			x = h * ((double)i_int - 0.5);
			sum += f(x);
			if (myid == 2)
				printf("aquí myid: %d i_int: %d i: %d\n", myid, i_int, i);
		}
		mypi = h * sum;
		printf("\nProcessor: %d, mypi: %.16f\n", myid, mypi);
		if(myid == 0) {
			print_time();
			pi = mypi;
		
			for (i = 1; i < numprocs; i++) {
				source = i;           /* MPI process number range is [0,p-1] */
				MPI_Recv(&mypi, 1, MPI_DOUBLE, source, tag,
				 MPI_COMM_WORLD, &status);
				pi += mypi;
			}
			endwtime = MPI_Wtime();
			printf("\n\033[22;32mpi is approximately %.16f, Error is %.16f\n\e[m", pi, fabs(pi - PI25DT));
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

void print_time(void) {
	char buff[100];
	time_t now = time (0);
	strftime (buff, 100, "%Y-%m-%d %H:%M:%S", localtime (&now));
	printf ("\033[22;32mTIME: %s\n\e[m", buff);
}

int calculate_block(int block, int block_size) {
	int up_limit = block * block_size;
	int down_limit = up_limit - block_size;
	int mypi, n, h, sum, x;

	n = 100;
	h   = 1.0 / (double) n;
	sum = 0.0;
	//block_size = n/numprocs;
	//for (i = myid + 1; i <= block_size; i += 1) {
		//x = h * ((double)i - 0.5);
		sum += f(x);
	//}
	mypi = h * sum;

	return mypi;
}