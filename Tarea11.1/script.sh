date
export OMP_NUM_THREADS=2
mpirun -n 1 ./matrix 1000 40 10 0
mpirun -n 1 ./matrix 2000 40 10 0
mpirun -n 1 ./matrix 3000 40 10 0
mpirun -n 1 ./matrix 4000 40 10 0
mpirun -n 1 ./matrix 5000 40 10 0
mpirun -n 2 ./matrix 1000 40 10 0
mpirun -n 2 ./matrix 2000 40 10 0
mpirun -n 2 ./matrix 3000 40 10 0
mpirun -n 2 ./matrix 4000 40 10 0
mpirun -n 2 ./matrix 5000 40 10 0
