mpicc -fopenmp -o matrix matrix-MPI-OpenMP-PavelNichita.c
date
mpirun -n 2 ./matrix 1000 40 10 0 1
mpirun -n 2 ./matrix 2000 40 10 0 1
mpirun -n 2 ./matrix 3000 40 10 0 1
mpirun -n 2 ./matrix 4000 40 10 0 1
mpirun -n 2 ./matrix 5000 40 10 0 1

