//
// Celestial/Sim.hpp
// N-Body-Simulator
//

#ifndef CELESTIAL_SIM_HPP
#define CELESTIAL_SIM_HPP

#include <random>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include "../Constant.hpp"
#include "../Utils.hpp"
#include "Body.hpp"

namespace Celestial
{



    class Sim
    {
    public:

        typedef std::vector<Celestial::Body> Container;

    public:
        // Constructor
        Sim(sf::RenderWindow& window);

        // Methods
        void    addCelestialBody(Body& b);
        void    handleEvent(const sf::Event& event);
        void    populate(int number, int center_x, int center_y, int radius);
        void    update(sf::Time dt);

        bool    isRunning() const;
        void    run();
        void    stop();

        void    render() const;

    protected:
        // Internal handling
        void    handleCollisions();

    private:

        bool                mIsRunning;
        bool                mouseHeldDown;

        sf::RenderWindow*   mLinkedWindow;
        Container           mPlanetArray;

    };



}

#endif // CELESTIAL_SIM_HPP
