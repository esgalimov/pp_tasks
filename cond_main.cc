#include "conductivity.hh"
#include <fstream>
#include <string>
#include <filesystem>

namespace tc = thermal_conductivity;

int main(int argc, char* argv[]) try {
    tc::thermal_conductivity_solver_t::init(argc, argv);

    auto file_folder = std::filesystem::absolute(__FILE__).parent_path();
    int rank = tc::thermal_conductivity_solver_t::get_rank();

    std::string output_filename = file_folder.string() + "/out/node_" + std::to_string(rank) + ".txt";

    std::ofstream fout(output_filename);
    
    tc::thermal_conductivity_solver_t::process_steps();
    tc::thermal_conductivity_solver_t::dump_res(fout);

    return 0;
}
catch (const std::exception& exc) {
    std::cerr << "Something wrong: " << exc.what() << std::endl;
}