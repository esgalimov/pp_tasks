#ifndef MY_THERMAL_CONDUCTIVITY
#define MY_THERMAL_CONDUCTIVITY

#include <mpi.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <algorithm>

constexpr double EPS = 0.00000001;
constexpr int ALIGNMENT = 12;


class thermal_conductivity_solver_t final {

    int rank = 0;
    int size = 0;

    double tau = 0.0;
    double   h = 0.0;

    long n_tau = 0;
    long n_h   = 0;

    using points_it_t      = typename std::vector<std::vector<double>>::iterator;
    using vec_vec_double_t = typename std::vector<std::vector<double>>;

    vec_vec_double_t points;
    vec_vec_double_t heterogeneity;

    points_it_t step_it;
    int curr_step = 1;


    void recs_neighbour_points() {

    }

    void send_neighbour_points() {

    }

    

    void get_time_initial_func() {
        for (auto& vec : points) vec[0] = 1;
    }

    void get_coord_initial_func() {}

    void get_heterogeneity_func() {}

    void process_single_step() {
        for (int i = 1; i < n_h - 1; ++i) {
            double deriv1 = (points[curr_step - 1][i - 1] - points[curr_step - 1][i + 1]) * tau / (2 * h);
            double deriv2 = (points[curr_step - 1][i + 1] - 2 * points[curr_step - 1][i] + points[curr_step - 1][i - 1]) * tau * tau / (2 * h * h);

            points[curr_step][i] = points[curr_step - 1][i] + tau * heterogeneity[curr_step - 1][i] + deriv1 + deriv2;
        }
    }

public:

    void process_steps() {

        if (size == 1) {
            while (curr_step < n_tau) {
                process_single_step();
                curr_step++;
            }
        }
    }

    void dump_res(std::ostream& os) const {
        for (auto& vec : points) {
            for (auto& pnt : vec) 
                os << std::setw(ALIGNMENT) << pnt << " ";
            os << std::endl;
        }
    }

    thermal_conductivity_solver_t(int argc, char* argv[]) {
        if (argc == 5) {
            tau   = std::stod(argv[1]);
            n_tau = std::stol(argv[2]);
            h     = std::stod(argv[3]);
            n_h   = std::stol(argv[4]);
        }
        else throw std::runtime_error("Must be 4 arguments: tau, n_tau, h, n_h.");

        if (n_tau <= 4 || n_h <= 4) 
            throw std::runtime_error("n_tau, n_h must be bigger than 4");
        if (tau < EPS || h < EPS)
            throw std::runtime_error("tau, h must be bigger than EPS = 0.00000001");
             
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);


        long r_h = n_h % size;

        if (r_h) {  
            n_h   += r_h;
            std::cerr << "Warning: n_h % size != 0 => n_h += remainder :)" << std::endl;
        }

        heterogeneity.resize(n_tau++);
        points.resize(n_tau);

        for (auto& vec : points)        vec.resize(n_h / size);
        for (auto& vec : heterogeneity) vec.resize(n_h / size);

        step_it = points.begin();

        #ifdef DEBUG
           
        #endif

        get_time_initial_func();
        get_coord_initial_func();
        get_heterogeneity_func();
    }

    
    ~thermal_conductivity_solver_t() {
        MPI_Finalize();
    }
};


#endif
