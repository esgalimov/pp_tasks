# Numerical solution of the transport equation using MPI technology

```
cmake -B build
./run.sh
```
You can change string in run.sh like this:
```
mpirun -n <nodes> ./build/cond_counter  <tau> <time-steps> <h> <coordinate-steps>
```
