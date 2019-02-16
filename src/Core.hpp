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

    //
    std::weak_ptr<const Celestial::Body> selectedBody;
    std::weak_ptr<const Celestial::Body> lockedOnBody;
    sf::VertexArray SAline;


    // Temporary Text and font
    sf::Text                mStats;
    sf::Text                mControls;
    sf::Font                mFont;

    sf::Text                mOverlay;

};

#endif // CORE_HPP
