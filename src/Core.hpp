//
// Core.hpp
// N-Body-Simulator
//

#ifndef CORE_HPP
#define CORE_HPP

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

    void    update(sf::Time dt);
    void    processInput();
    void    render();

private:

    static const sf::Time	TimePerFrame;

    sf::RenderWindow        mMainWindow;
    Celestial::Sim          mSimulator;

    // mouse related
    sf::Vector2i            mousePreviousPosition;
    bool                    moving;

    // window related
    float                   zoom;
    sf::View                view;

};





#endif // CORE_HPP
