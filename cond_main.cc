#include "conductivity.hh"
#include <fstream>
#include <string>
#include <filesystem>

int main(int argc, char* argv[]) try {
    thermal_conductivity_solver_t conduc(argc, argv);

    auto file_folder = std::filesystem::absolute(__FILE__).parent_path();

    std::string output_filename = file_folder.string() + "/out/node_" + std::to_string(conduc.get_rank()) + ".txt";

    std::ofstream fout(output_filename);

    conduc.process_steps();

    conduc.dump_res(fout);

    return 0;
}
catch (const std::exception& exc) {
    std::cerr << "Something wrong: " << exc.what() << std::endl;
}