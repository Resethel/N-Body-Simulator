//
// Core.hpp
// N-Body-Simulator
//

#ifndef CORE_HPP
#define CORE_HPP

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include "Celestial/Sim.hpp"
#include "Celestial/Body.hpp"

class Core
{
public:

    Core();

    void run();

private:
    // Internal Handling
    void    update(sf::Time dt);
    void    processInput();
    void    render();

private:

    static const sf::Time	TimePerFrame;

    // Simulation
    sf::RenderWindow        mMainWindow;
    Celestial::Sim          mSimulator;
    float                   mTimeStepMultiplier;

    // mouse related
    sf::Vector2f            oldMousePosition;
    bool                    draggingView;

    // window related
    float                   zoom;
    sf::View                view;

    // For Information display
    Celestial::Body::ConstWeakPtr   selectedBody;
    Celestial::Body::ConstWeakPtr   lockedOnBody;
    sf::VertexArray                 SAline; // To strongest attractor
    sf::VertexArray                 velocityLine; // Velocity


    // Temporary Text and font
    sf::Text                mStats;
    sf::Text                mControls;
    sf::Font                mFont;

    sf::Text                mOverlay;

};

#endif // CORE_HPP
