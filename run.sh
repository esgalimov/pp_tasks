cd build
make
cd ..

rm -rf out

mkdir out

#time mpirun -n 4 ./build/cond_counter  0.0001 1000001 0.01 100000
mpirun -n 4 ./build/cond_counter  0.001 50 0.01 16


python3 draw.py
