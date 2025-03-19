#include "pi_counter.hh"

int main(int argc, char* argv[]) {
    pi_counter_t picnt(argc, argv);

    std::optional<double> pi = picnt.get_pi();

    std::cout.precision(20);

    if (pi.has_value()) std::cout << "pi = " << pi.value() << std::endl;
    
    return 0;
}