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
        void    update(sf::Time dt);
        void    handleEvent(const sf::Event& event);

        void    addCelestialBody(Body& b);
        void    removeCelestialBody(const unsigned& ind);
        void    populate(size_t number, int center_x, int center_y, int radius);

        bool    isRunning() const;
        void    run();
        void    stop();

        void    render() const;

        void    explodePlanet(const int& ind);

    protected:
        // Internal handling
        void    handleCollisions();

    private:

        sf::RenderWindow*   mLinkedWindow;
        Container           mPlanetArray;

        bool                mIsRunning;
        bool                mouseHeldDown;

        // Temporary celestial Body
        Body::Ptr           mTempBody;
        sf::Vertex          mSpeedVector[2];          

    };



}

#endif // CELESTIAL_SIM_HPP
