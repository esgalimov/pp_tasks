cd build
make
cd ..

rm -rf out

mkdir out

time mpirun -n 4 ./build/cond_counter  0.0001 1000001 0.01 100000


python3 draw.py

#точек 100000; шагов по времени 1000001; шаг по т 0.0001; шаг по икс 0.01
