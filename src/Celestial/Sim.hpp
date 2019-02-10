//
// Celestial/Sim.hpp
// N-Body-Simulator
//

#ifndef CELESTIAL_SIM_HPP
#define CELESTIAL_SIM_HPP

#include <random>
#include <vector>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include "../Constant.hpp"
#include "../Utils.hpp"
#include "../GFX/Explosion.hpp"
#include "Body.hpp"

#include "../GFX/Trail.hpp"

namespace Celestial
{



    class Sim
    {
    public:

        typedef std::vector<Celestial::Body>    BodyContainer;
        typedef std::vector<gfx::Explosion>     ExplosionContainer;

    public:
        // Constructor
        Sim(sf::RenderWindow& window);

        // Methods
        void    update(sf::Time dt);
        void    handleEvent(const sf::Event& event);

        void    addCelestialBody(Body& b);
        void    addCelestialBody(double x, double y, double vel_x, double vel_y, double mass);
        void    removeCelestialBody(const size_t& ind);
        void    populate(size_t number, int center_x, int center_y, int radius);

        void    addExplosion(gfx::Explosion& expl);
        void    removeExplosion(const size_t& ind);

        bool    isRunning() const;
        void    run();
        void    stop();
        void    reset();

        void    render() const;



        // getters
        unsigned            getBodyCount() const;
        double              getTotalMass() const;
        unsigned long long  getSimulationStep() const;

    protected:
        // Internal handling
        void    physicalResolution();
        void    effectsResolution();

        void    dislocateBody(const int& ind);

    private:

        sf::RenderWindow*   mLinkedWindow;

        BodyContainer       mPlanetArray;
        ExplosionContainer  mExplosionArray;

        // Statistics
        unsigned            mBodyCount;
        double              mTotalMass;
        unsigned long long  mSimulationStep;

        bool                mIsRunning;
        bool                mouseHeldDown;



        // Temporary celestial Body
        Body::Ptr           mTempBody;
        sf::Vertex          mSpeedVector[2];

        // Test Trail
        gfx::Trail          mTrail;
        Body*               mTrailedBody;

    };



}

#endif // CELESTIAL_SIM_HPP
