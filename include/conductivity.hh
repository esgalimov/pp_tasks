#ifndef MY_THERMAL_CONDUCTIVITY
#define MY_THERMAL_CONDUCTIVITY

#include <mpi.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <memory>

constexpr double EPS = 0.00000001;
constexpr int ALIGNMENT = 12;
constexpr int DEFAULT_TAG     = 0;


namespace thermal_conductivity {
class thermal_conductivity_solver_t final {

    int rank = 0;
    int size = 0;

    double tau = 0.0;
    double   h = 0.0;

    long n_tau     = 0;
    long n_tau_all = 0;
    long n_h       = 0;
    long n_h_all   = 0;

    using vec_vec_double_t = typename std::vector<std::vector<double>>;

    vec_vec_double_t points;

    int curr_step = 1;

    void get_time_initial_func() {
        if (rank == 0)
            for (auto& vec : points) vec[0] = 1;
    }

    void get_coord_initial_func() {}

    void get_heterogeneity_func() {}

    void process_single_step() {
        for (int i = 1; i < n_h - 1; ++i) 
            process_single_iter(points[curr_step - 1][i - 1], points[curr_step - 1][i + 1], i);
    }

    void process_on_single_node() {
        while (n_tau > 1) {
            process_single_step();
            after_single_step();
        }
    }

    double get_curr_heterogeneity(int i) {
        return 0;
    }

    void process_single_iter(double prev, double next, int i) {
        double deriv1 = (prev - next) * tau / (2 * h);
        double deriv2 = (next - 2 * points[curr_step - 1][i] + prev) * tau * tau / (2 * h * h);

        points[curr_step][i] = points[curr_step - 1][i] + tau * get_curr_heterogeneity(i) + deriv1 + deriv2;
    }

    void process_single_step_on_non_outermost_node() {
        MPI_Request reqs[4] = {};

        double recv_first = 0.0;
        double recv_last  = 0.0;
                
        int stat_send1 = MPI_Isend(&points[curr_step - 1][0],       1, MPI_DOUBLE, rank - 1, DEFAULT_TAG, MPI_COMM_WORLD, &reqs[0]);
        int stat_send2 = MPI_Isend(&points[curr_step - 1][n_h - 1], 1, MPI_DOUBLE, rank + 1, DEFAULT_TAG, MPI_COMM_WORLD, &reqs[1]);

        int stat_recv1 = MPI_Irecv(&recv_first, 1, MPI_DOUBLE, rank - 1, DEFAULT_TAG, MPI_COMM_WORLD, &reqs[2]);
        int stat_recv2 = MPI_Irecv(&recv_last,  1, MPI_DOUBLE, rank + 1, DEFAULT_TAG, MPI_COMM_WORLD, &reqs[3]);

        process_single_step();

        MPI_Waitall(4, reqs, MPI_STATUS_IGNORE);

        if (stat_recv1 || stat_recv2 || stat_send1 || stat_send2) 
            throw std::runtime_error("Something wrong in MPI_Isend or MPI_Irecv");

        process_single_iter(recv_first, points[curr_step - 1][1], 0);
        process_single_iter(points[curr_step - 1][n_h - 2], recv_last, n_h - 1);
    }

    void process_single_step_on_outermost_node(int send_i) {
        MPI_Request reqs[2] = {};

        double recv  = 0.0;

        int recv_send_rank = rank + 1;
        
        if (send_i == 0) recv_send_rank = rank - 1;

        int stat_send = MPI_Isend(&points[curr_step - 1][send_i], 1, MPI_DOUBLE, recv_send_rank, DEFAULT_TAG, MPI_COMM_WORLD, &reqs[0]);
        int stat_recv = MPI_Irecv(&recv,  1, MPI_DOUBLE, recv_send_rank, DEFAULT_TAG, MPI_COMM_WORLD, &reqs[1]);

        process_single_step();

        MPI_Waitall(2, reqs, MPI_STATUS_IGNORE);

        if (stat_recv || stat_send) 
            throw std::runtime_error("Something wrong in MPI_Isend or MPI_Irecv");

        if (send_i == 0) process_single_iter(recv, points[curr_step - 1][1], 0);
        else             process_single_iter(points[curr_step - 1][n_h - 2], recv, n_h - 1);
    }   

    void process_on_multiple_nodes() {
        while (n_tau > 1) {
            if (rank != 0 && rank != size - 1) process_single_step_on_non_outermost_node();
            else if (rank == 0)                process_single_step_on_outermost_node(n_h - 1);
            else                               process_single_step_on_outermost_node(0);

            after_single_step();
        }
    }

    void after_single_step() {
        n_tau--;
        #ifdef DEBUG
            curr_step++;
        #else
            points[0].swap(points[1]);
        #endif
    }

    void init_all(int argc, char* argv[]) {
        if (argc == 5) {
            tau       = std::stod(argv[1]);
            n_tau_all = std::stol(argv[2]);
            h         = std::stod(argv[3]);
            n_h_all   = std::stol(argv[4]);
        }
        else throw std::runtime_error("Must be 4 arguments: tau, n_tau_all, h, n_h_all.");
     
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if (n_tau_all < 1 || n_h_all / size < 4) 
            throw std::runtime_error("n_h / size must be bigger than 4, n_tau must be bigger than 0");
        if (tau < EPS || h < EPS)
            throw std::runtime_error("tau, h must be bigger than EPS = 0.00000001");

        #ifdef DEBUG
        points.resize(++n_tau_all);
        #else
        points.resize(2);
        #endif
  
        n_tau = n_tau_all;
        n_h = (rank == size - 1) ? n_h_all / size + n_h_all % size : n_h_all / size;
        
        for (auto& vec : points) vec.resize(n_h);

        get_time_initial_func();
        get_coord_initial_func();
    }

    void process_steps_private() {
        if (size == 1) process_on_single_node();
        else           process_on_multiple_nodes();
        
    }

    void dump_res_private(std::ostream& os) const {
        #ifdef DEBUG
        for (auto& vec : points) {
            for (auto& pnt : vec) 
                os << std::setw(ALIGNMENT) << pnt << " ";
            os << std::endl;
        }
        #else
        for (auto& pnt : points[0]) 
            os << std::setw(ALIGNMENT) << pnt << " ";
        os << std::endl;
        #endif
    }

    thermal_conductivity_solver_t() {}


public:

    static thermal_conductivity_solver_t& get_instance() {
        static thermal_conductivity_solver_t instance;
        return instance;
    }

    static void init(int argc, char* argv[]) {
        if (get_instance().size != 0)
            throw std::runtime_error("Multiple init thermal_conductivity_solver_t");

        get_instance().init_all(argc, argv);
    }
    
    static void process_steps() {
        get_instance().process_steps_private();
        
    }

    static void dump_res(std::ostream& os) {
        get_instance().dump_res_private(os);
    }

    
    static int get_rank() { return get_instance().rank; }
    static int get_size() { return get_instance().size; }


    thermal_conductivity_solver_t(const thermal_conductivity_solver_t&)            = delete;
    thermal_conductivity_solver_t(thermal_conductivity_solver_t&&)                 = delete;
    thermal_conductivity_solver_t& operator=(const thermal_conductivity_solver_t&) = delete;
    thermal_conductivity_solver_t& operator=(thermal_conductivity_solver_t&&)      = delete;

    ~thermal_conductivity_solver_t() {
        if (size != 0) MPI_Finalize();
    }
};

} // <  ---- namespace thermal_conductivity
#endif
