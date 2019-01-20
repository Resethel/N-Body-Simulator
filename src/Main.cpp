//
// Main.cpp
// N-Body-Simulator
//

#include <exception>
#include <iostream>

#include <SFML/System.hpp>

#include "Core.hpp"


int main(int argc, char** argv)
{
    try
    {
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
