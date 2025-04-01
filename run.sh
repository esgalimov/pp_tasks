cd build
make
cd ..

rm -rf out

mkdir out


mpirun -n 4 ./build/cond_counter  0.001 50 0.01 24


# В Release версии:
# time mpirun -n 1 ./build/cond_counter  0.0001 1000001 0.01 100000
# time mpirun -n 2 ./build/cond_counter  0.0001 1000001 0.01 100000
# time mpirun -n 3 ./build/cond_counter  0.0001 1000001 0.01 100000
# time mpirun -n 4 ./build/cond_counter  0.0001 1000001 0.01 100000

# real    4m42,085s
# user    4m41,244s
# sys     0m0,021s

# real    2m29,293s
# user    4m58,023s
# sys     0m0,047s

# real    2m1,132s
# user    6m2,590s
# sys     0m0,060s

# real    1m51,647s
# user    7m25,480s
# sys     0m0,073s


python3 draw.py
