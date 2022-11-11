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
    const double G = REAL_G * 1e11;
    const double SPEED_OF_LIGHT = 299792458;  // in m/s

    // Calculation constants
    const double MIN_DISTANCE_FOR_CALCULATION = 10 * std::numeric_limits<double>::epsilon();
    const double SOFTENING_FACTOR             = std::numeric_limits<double>::epsilon();

    // BODY_TYPE_MASS
    const double ROCKY_LIMIT        = 15;
    const double TERRESTIAL_LIMIT   = 50;
    const double GAS_GIANT_LIMIT    = 400;
    const double SMALL_STAR_LIMIT   = 800;
    const double STAR_LIMIT         = 1500;
    const double BIG_STAR_LIMIT     = 3500;

    // COLLISIONS
    const double COLLISION_ABSORPTION_RATIO = 1.01;

    // ROCHE LIMIT
    const double ROCHE_LIMIT_MULTIPLIER = 4.6; // Multiplier for simulation
    const double ROCHE_LIMIT_MIN_MASS_RATIO = 0.4; // Minimum mass ratio for tidal force to rip a celestial body

    // PLANET EXPLOSION
    const double EXPLOSION_PLANET_SPEED_MULTIPLIER  = 1.05; // adjust explosion speed on regular
    const double EXPLOSION_ROCHE_SPEED_MULTIPLIER   = 0.98; // adjust explosion speed on roche limit explosion

    // SCHWARZSCHILD RADIUS MULTIPLIER
    const double SCHWARZSCHILD_MULTIPLIER   = 0.001;

    // GFX: EXPLOSIONS, BREAKING, ETC
    const double MINIMUM_MASS_FOR_DIVISION  = 3;
    const double EXPLOSION_FLASH_SIZE       = 10;
    const double EXPLOSION_LIGHT_STRENGTH   = 50;
    const size_t EXPLOSION_POINT_COUNT      = 50;


    // BOUNDARIES
    const double DEFAULT_BOUNDARY_RADIUS = 1e4;

    // Graphics
    const double TRAIL_FADE_RATIO = 0.3;

    // GUI
    const float ZOOM_INCREMENT              = 0.1f;
    const float TIMESTEP_INCREMENT          = 0.5f;
    const double VELOCITY_LINE_MULTIPLIER   = 2.5;

    // OTHER
    const float  SLINGSHOT_MULTIPLIER = 2.5;

    //SYSTEM GENERATOR
    const double MASS_MULTIPLIER = 130;
    const double NUMBER_OF_OBJECT_MULTIPLIER = 0.4;
}

#endif // CONSTANT_HPP
