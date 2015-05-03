clear;
export OMP_NUM_THREADS=1
echo "Procesadores 1"
./matrizopenmp 1000 40 10 0
export OMP_NUM_THREADS=2
echo "Procesadores 2"
./matrizopenmp 1000 40 10 0
export OMP_NUM_THREADS=3
echo "Procesadores 3"
./matrizopenmp 1000 40 10 0
export OMP_NUM_THREADS=4
echo "Procesadores 4"
./matrizopenmp 1000 40 10 0
