//
// Core.cpp
// N-Body-Simulator
//

#include "Core.hpp"

const sf::Time Core::TimePerFrame = sf::seconds(1.f/60.f);

Core::Core()
: mMainWindow(sf::VideoMode(800,600), "N-Body-Simulator", sf::Style::Close)
{}


////////// Methods

void Core::run()
{

    sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

    unsigned updateRealised;
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


        mMainWindow.clear(sf::Color::Black);


        mMainWindow.display();
    }
}


////////////

void Core::update(sf::Time dt)
{

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
    }
}

void Core::render()
{
    mMainWindow.clear(sf::Color::Black);


    // End of the current frame
    mMainWindow.display();
}
