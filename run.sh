cd build
make
cd ..

rm -rf out

mkdir out

mpirun -n 3 ./build/cond_counter  0.001 50 0.01 12


python3 draw.py
