//
// Constant.hpp
// N-Body-Simulation
//

#ifndef CONSTANT_HPP
#define CONSTANT_HPP


namespace CONSTANT
{
    // Gravitational Constant
    const double G = 6.673e1;

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


}

#endif // CONSTANT_HPP
