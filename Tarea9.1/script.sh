export OMP_NUM_THREADS=4
echo "Comparacion 1" > cpi-vector-desktop.txt
./cpi-vector >> cpi-vector-desktop.txt
./cpi-vector2 >> cpi-vector-desktop.txt
echo "Comparacion 2" >> cpi-vector-desktop.txt
./cpi-vector >> cpi-vector-desktop.txt
./cpi-vector2 >> cpi-vector-desktop.txt
echo "Comparacion 3" >> cpi-vector-desktop.txt
./cpi-vector >> cpi-vector-desktop.txt
./cpi-vector2 >> cpi-vector-desktop.txt
echo "Comparacion 4" >> cpi-vector-desktop.txt
./cpi-vector >> cpi-vector-desktop.txt
./cpi-vector2 >> cpi-vector-desktop.txt
