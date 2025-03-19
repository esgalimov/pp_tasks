#ifndef MY_PI_COUNTER
#define MY_PI_COUNTER

#include <mpi.h>
#include <random>
#include <iostream>
#include <optional>
#include <string>
#include <exception>
	

constexpr int  MAIN_NODE       = 0;
constexpr int  DEFAULT_TAG     = 0;


class pi_counter_t final {
    long POINTS_PER_NODE = 1000;

    int rank = 0;
    int size = 0;
    long cnt = 0;

    void count_random_points_in_circle_quarter() {
        std::random_device rd; 
        std::mt19937 gen(rd()); 
        std::uniform_real_distribution<> dis(0.0, 1.0);

        for (int n = 0; n < POINTS_PER_NODE; ++n) {
            double x = dis(gen);
            double y = dis(gen);

            if (x * x + y * y <= 1) cnt++;
        }
    }

    void send_to_main() {
        if (rank != 0) {
            int status = MPI_Send(&cnt, 1, MPI_LONG, MAIN_NODE, DEFAULT_TAG, MPI_COMM_WORLD); 

            if (status != MPI_SUCCESS) 
                std::cerr << "Something wrong; MPI_Send status = " << status << std::endl;
        }
    }

public:
    pi_counter_t(int argc, char* argv[]) {
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        try {
            if (argc > 1) 
                POINTS_PER_NODE = std::stol(argv[1]);
        } catch (std::exception exc) {
            std::cerr << "Something wrong in command line argument POINTS_PER_NODE: " << exc.what() << std::endl;
        }

        count_random_points_in_circle_quarter();

        send_to_main();

        #ifndef NDEBUG
            std::cout << "Hello! I am node " << rank << " and generated " << cnt << "in circle" << std::endl;
        #endif
    }

    
    std::optional<double> get_pi() {
        long sum = 0;
        long buf = 0;

        if (rank == 0) {
            sum += cnt;
            for (int i = 1; i < size; ++i) {
                int status = MPI_Recv(&buf, 1, MPI_LONG, i, DEFAULT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                if (status != MPI_SUCCESS) 
                    std::cerr << "Something wrong; MPI_Recv status = " << status << std::endl;

                sum += buf;
            }      
            return 4 * (double) sum / (double) (POINTS_PER_NODE * size);
        }   
        return std::nullopt;
    }

    ~pi_counter_t() {
        MPI_Finalize();
    }
};


#endif