//
// Core.hpp
// N-Body-Simulator
//

#ifndef CORE_HPP
#define CORE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

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


};





#endif // CORE_HPP
