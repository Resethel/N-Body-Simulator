//
// Main.cpp
// N-Body-Simulator
//

#include <exception>
#include <iostream>
#include <random>

#include <SFML/System.hpp>

#include "Utils.hpp"
#include "Core.hpp"


int main(int argc, char** argv)
{
    try
    {
        // Initializing the random number generator
        std::random_device r;
        std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
        utils::rng.seed(seed);

        // Lunching the Simulation
        Core core;
        core.run();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;
}
