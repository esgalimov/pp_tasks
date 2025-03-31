#include <chrono>
#include <iostream>
#include "mpi.h"


constexpr int DEFAULT_TAG = 0;
constexpr int MAIN_NODE   = 0;
constexpr int SLAVE_NODE  = 1;
constexpr int ITERS       = 100;


int main(int argc, char* argv[]) {
    int size = 0;
    int rank = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    long buf = 0;
    long num = 123;

    if (rank == MAIN_NODE) {
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < ITERS; ++i) {
            MPI_Ssend(&num, 1, MPI_LONG, SLAVE_NODE, DEFAULT_TAG, MPI_COMM_WORLD);
            MPI_Recv(&buf, 1, MPI_LONG, SLAVE_NODE, DEFAULT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        double tm = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now() - start).count();

        std::cout << tm / 100 << " microseconds" << std::endl;
        
    }
    else if (rank == SLAVE_NODE) {
        for (int i = 0; i < ITERS; ++i) {
            MPI_Recv(&buf, 1, MPI_LONG, MAIN_NODE, DEFAULT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Ssend(&num, 1, MPI_LONG, MAIN_NODE, DEFAULT_TAG, MPI_COMM_WORLD);
        }
    }


    MPI_Finalize();

    return 0;
}