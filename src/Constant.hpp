//
// Constant.hpp
// N-Body-Simulation
//

#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include <limits>


namespace CONSTANT
{
    // Gravitational Constant
    const double REAL_G = 6.673e-11;
    const double G = 6.673e1;

    // Calculation constants

    const double MIN_DISTANCE_FOR_CALCULATION = 10 * std::numeric_limits<double>().epsilon();

    // BODY_TYPE_MASS
    const double ROCKY_LIMIT = 15;
    const double TERRESTIAL_LIMIT = 50;
    const double GAS_GIANT_LIMIT = 400;
    const double SMALL_STAR_LIMIT = 800;
    const double STAR_LIMIT = 1500;
    const double BIG_STAR_LIMIT = 3500;

    // ROCHE_LIMIT
    const double ROCHE_LIMIT_MULTIPLIER = 4.6; // Multiplier for simulation
    const double ROCHE_LIMIT_MIN_MASS_RATIO = 0.4; // Minimum mass ration for tidal force to rip a celestial body
    const double ROCHE_LIMIT_SPEED_MULTIPLIER = 3; // adjust explosion speed on roche limite explosion

    // EXPLOSIONS, BREAKING, ETC
    const double MINIMUM_MASS_FOR_DIVISION = 3;
    const double EXPLOSION_FLASH_SIZE = 10;
    const double EXPLOSION_LIGHT_STRENGTH = 50;
    const size_t EXPLOSION_POINT_COUNT = 50;


}

#endif // CONSTANT_HPP
