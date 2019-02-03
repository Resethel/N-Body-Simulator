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
<<<<<<< HEAD
        void    addCelestialBody(Body& b);
        void    removeCelestialBody(const unsigned& ind);
        void    handleEvent(const sf::Event& event);
        void    populate(size_t number, int center_x, int center_y, int radius);
        void    update(sf::Time dt);
=======
        void    update(sf::Time dt);
        void    handleEvent(const sf::Event& event);

        void    addCelestialBody(Body& b);
        void    addCelestialBody(double x, double y, double vel_x, double vel_y, double mass);
        void    removeCelestialBody(const unsigned& ind);
        void    populate(size_t number, int center_x, int center_y, int radius);
>>>>>>> develop

        bool    isRunning() const;
        void    run();
        void    stop();
<<<<<<< HEAD
=======
        void    reset();
>>>>>>> develop

        void    render() const;

        void    explodePlanet(const int& ind);

<<<<<<< HEAD
=======
        // getters
        unsigned            getBodyCount() const;
        double              getTotalMass() const;
        unsigned long long  getSimulationStep() const;

>>>>>>> develop
    protected:
        // Internal handling
        void    handleCollisions();

    private:

<<<<<<< HEAD
        bool                mIsRunning;
        bool                mouseHeldDown;

        sf::RenderWindow*   mLinkedWindow;
        Container           mPlanetArray;
=======
        sf::RenderWindow*   mLinkedWindow;
        Container           mPlanetArray;

        // Statistics
        unsigned            mBodyCount;
        double              mTotalMass;
        unsigned long long  mSimulationStep;

        bool                mIsRunning;
        bool                mouseHeldDown;



        // Temporary celestial Body
        Body::Ptr           mTempBody;
        sf::Vertex          mSpeedVector[2];
>>>>>>> develop

    };



}

#endif // CELESTIAL_SIM_HPP
