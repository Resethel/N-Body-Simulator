//
// Core.cpp
// N-Body-Simulator
//

#include "Core.hpp"

#include <iostream>

const sf::Time Core::TimePerFrame = sf::seconds(1.f/60.f);

Core::Core()
: mMainWindow(sf::VideoMode(1600,1200), "N-Body-Simulator", sf::Style::Close)
{}


////////// Methods

void Core::run()
{

    sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

    unsigned updateRealised;

    // Test planets
    Celestial::Body planet1(50,50,100,0,5.f);
    Celestial::Body planet2(800,600,0,0,6065500000.f);
    Celestial::Body planet3(600,1000,270,50,4500);
    Celestial::Body planet4(600,200,250,-50,4500000000);


    planets.push_back(std::move(planet1));
    planets.push_back(std::move(planet2));
    planets.push_back(std::move(planet3));
    planets.push_back(std::move(planet4));

    // Run the program as long as the window is open
    while (mMainWindow.isOpen())
    {
        // Update cycle
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            // Ensure we don't get stuck if we can't reach our intended number
            // of updates per second
            if (updateRealised++ < 60)
            {
                processInput();
                update(TimePerFrame);
            }
        }
        updateRealised = 0;

        render();
    }
}


////////////

void Core::update(sf::Time dt)
{
    for (int i = 0; i < planets.size(); i++)
    {
      planets[i].resetForce();
      //Notice-2 loops-->N^2 complexity
      for (int j = 0; j < planets.size(); j++)
      {
        if (i != j)
            planets[i].addForce(planets[j]);
      }
    }

    //Then, loop again and update the bodies using timestep dt
    for (int i = 0; i < planets.size(); i++)
    {
        planets[i].update(dt);
    }

}

void Core::processInput()
{
    // Check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (mMainWindow.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
        {
            mMainWindow.close();
        }
        if (event.type == sf::Event::KeyReleased)
        {
            //update(TimePerFrame);
        }
    }
}

void Core::render()
{
    mMainWindow.clear(sf::Color::Black);

    for(auto& body : planets)
    {
        mMainWindow.draw(body);
    }

    // End of the current frame
    mMainWindow.display();
}
