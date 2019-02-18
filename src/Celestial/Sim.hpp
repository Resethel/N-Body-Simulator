//
// Celestial/Sim.hpp
// N-Body-Simulator
//

#ifndef CELESTIAL_SIM_HPP
#define CELESTIAL_SIM_HPP

#include <algorithm>
#include <cmath>
#include <memory>
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

        typedef std::vector<Body::Ptr>      BodyContainer;
        typedef std::vector<gfx::Explosion> ExplosionContainer;
        typedef std::vector<gfx::Trail>     TrailContainer;

    public:
        // Constructor
        Sim(sf::RenderWindow& window);

        // Methods
        // for Simulation
        void    handleEvent(const sf::Event& event);
        void    update(sf::Time dt);
        void    render() const;

        void    run();
        void    stop();
        void    reset();
        bool    isRunning() const;

        std::weak_ptr<const Body> getBodyAtPosition(sf::Vector2f pos) const;

        //// for Celestial bodies
        void    addCelestialBody(const Body& b, bool update_mass = true);
        void    addCelestialBody(double x, double y, double vel_x, double vel_y, double mass, bool update_mass = true);
        void    removeCelestialBody(const size_t& ind, bool update_mass = true);

        void    populate(size_t number, int center_x, int center_y, int radius);

        //// for Graphical Effects
        void    addExplosion(gfx::Explosion& expl);
        void    removeExplosion(const size_t& ind);

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

        // Containers
        BodyContainer       mPlanetArray;
        ExplosionContainer  mExplosionArray;
        TrailContainer      mTrailArray;

        // Statistics
        unsigned            mBodyCount;
        double              mTotalMass;
        unsigned long long  mSimulationStep;

        bool                mIsRunning;
        bool                mouseHeldDown;

        // Temporary celestial Body
        Body::Ptr           mTempBody;
        sf::Vertex          mSpeedVector[2];

    };



}

#endif // CELESTIAL_SIM_HPP
