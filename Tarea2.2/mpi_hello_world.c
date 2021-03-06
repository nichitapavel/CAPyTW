// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header in tact.
//
// An intro MPI hello world program that uses MPI_Init, MPI_Comm_size,
// MPI_Comm_rank, MPI_Finalize, and MPI_Get_processor_name.
//
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  double startwtime, endwtime;
  int i, j, k;
  int tam = 800;

  // Initialize the MPI environment. The two arguments to MPI Init are not
  // currently used by MPI implementations, but are there in case future
  // implementations might need the arguments.
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  // Print off a hello world message
  printf("Hello world from processor %s, rank %d out of %d processors\n",
         processor_name, world_rank, world_size);

  //Producto de Matriz

  int A[tam][tam];
  int B[tam][tam];
  int C[tam][tam];
  srand(time(NULL));

  for (i = 0; i < tam;i++) {
    for (j = 0; j < tam; j++) {
      A[i][j] = (int) rand() % 7;
      B[i][j] = (int) rand() % 3;
    }
  }

  startwtime = MPI_Wtime();

  for (i = 0; i < tam;i++) {
    for (j = 0; j < tam; j++) {
      C[i][j] = 0;
      for (k =0; k < tam; k++) {
        C[i][j] = C[i][j] + A[i][k] * B[k][j];
      }
    }
  }

  endwtime = MPI_Wtime();

  printf("Time: %f\n", (endwtime-startwtime));

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();

  //printf("Time %ld \n", time(NULL) - Atime);
}
