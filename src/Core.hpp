//
// Core.hpp
// N-Body-Simulator
//

#ifndef CORE_HPP
#define CORE_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

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


    std::vector<Celestial::Body> planets;


};





#endif // CORE_HPP
