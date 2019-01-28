//
// Core.cpp
// N-Body-Simulator
//

#include "Core.hpp"

#include <iostream>

const sf::Time Core::TimePerFrame = sf::seconds(1.f/60.f);

Core::Core()
: mMainWindow(sf::VideoMode(1600,1200), "N-Body-Simulator", sf::Style::Close)
, mSimulator(mMainWindow)
, mousePreviousPosition(sf::Mouse::getPosition(mMainWindow))
, moving(false)
, zoom(1)
, view(mMainWindow.getDefaultView())
{
    mMainWindow.setFramerateLimit(60);
    mSimulator.stop();
}


////////// Methods

void Core::run()
{

    sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

    unsigned updateRealised;

    mSimulator.populate(1000,800,600,1500);


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
    mSimulator.update(dt);
}

void Core::processInput()
{
    mousePreviousPosition = sf::Mouse::getPosition(mMainWindow);
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
            mSimulator.run();
        }

        if (event.type == sf::Event::MouseWheelScrolled)
        {
            // Ignore the mouse wheel unless we're not moving
            if (!moving)
            {
                // Determine the scroll direction and adjust the zoom level
                if (event.mouseWheelScroll.delta <= -1)
                    zoom = std::min(50.f, zoom + .1f);
                else if (event.mouseWheelScroll.delta >= 1)
                    zoom = std::max(.5f, zoom - .1f);

                // Update our view
                view.setSize(mMainWindow.getDefaultView().getSize()); // Reset the size
                view.zoom(zoom); // Apply the zoom level (this transforms the view)
                mMainWindow.setView(view);
            }
        }


    }
}

void Core::render()
{
    mMainWindow.clear(sf::Color(10, 10, 10));

    mSimulator.render();
    // End of the current frame
    mMainWindow.display();
}
