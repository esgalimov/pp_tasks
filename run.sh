cd build
make
cd ..

mpirun -n 1 ./build/cond_counter  0.001 30 0.01 10 > out.txt

python3 draw.py
