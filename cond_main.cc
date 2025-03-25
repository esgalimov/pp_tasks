#include "conductivity.hh"

int main(int argc, char* argv[]) {
    thermal_conductivity_solver_t conduc(argc, argv);

    conduc.process_steps();

    conduc.dump_res(std::cout);

    
    return 0;
}