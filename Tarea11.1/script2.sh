mpicc -fopenmp -o matrix matrix-MPI-OpenMP-PavelNichita.c
date
mpirun -n 1 ./matrix 2000 40 10 0 2
mpirun -n 1 ./matrix 2000 40 10 0 4
mpirun -n 2 ./matrix 2000 40 10 0 1
mpirun -n 4 ./matrix 2000 40 10 0 1
