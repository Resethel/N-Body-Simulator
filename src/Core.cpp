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
, mTimeStepMultiplier(1)
, oldMousePosition(sf::Mouse::getPosition(mMainWindow))
, draggingView(false)
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
                update(TimePerFrame * mTimeStepMultiplier);
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
            switch (event.key.code)
            {
                case sf::Keyboard::Space:
                    if(mSimulator.isRunning())
                        mSimulator.stop();
                    else
                        mSimulator.run();
                break;

                case sf::Keyboard::Up:
                    mTimeStepMultiplier = utils::clamp<float>(mTimeStepMultiplier+0.5f, 0.5, 10);
                break;

                case sf::Keyboard::Down:
                    mTimeStepMultiplier = utils::clamp<float>(mTimeStepMultiplier-0.5f, 0.5, 100);
                break;

                default:
                break;
            }

        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) or sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                and event.mouseButton.button == sf::Mouse::Left)
            {
                draggingView = true;
                oldMousePosition = mMainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            }
        }

        if (event.type == sf::Event::MouseButtonReleased)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                draggingView = false;
            }
        }

        if (event.type == sf::Event::MouseMoved)
        {
            if(draggingView)
            {
                const sf::Vector2f newMousePosition = mMainWindow.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                const sf::Vector2f deltaPos         = oldMousePosition - newMousePosition;

                // Move our view accordingly and update the window
                view.setCenter(view.getCenter() + deltaPos);
                mMainWindow.setView(view);

                // Save the new position as the old one
                // We're recalculating this, since we've changed the view
                oldMousePosition = mMainWindow.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
            }
        }

        if (event.type == sf::Event::MouseWheelScrolled)
        {
            // Ignore the mouse wheel unless we're not moving
            if (!draggingView)
            {
                oldMousePosition = mMainWindow.mapPixelToCoords(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y));

                // Determine the scroll direction and adjust the zoom level
                if (event.mouseWheelScroll.delta <= -1)
                    zoom = std::min(50.f, zoom + .1f);
                else if (event.mouseWheelScroll.delta >= 1)
                    zoom = std::max(.5f, zoom - .1f);

                // Update our view
                view.setSize(mMainWindow.getDefaultView().getSize()); // Reset the size
                view.zoom(zoom); // Apply the zoom level (this transforms the view)
                mMainWindow.setView(view);

                // Updating once again to adapt the zoom to the mouse position
                const sf::Vector2f newMousePosition = mMainWindow.mapPixelToCoords(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y));
                const sf::Vector2f deltaPos         = oldMousePosition - newMousePosition;
                view.setCenter(view.getCenter() + deltaPos);
                mMainWindow.setView(view);



            }
        }

        // Passing event to the simulator
        mSimulator.handleEvent(event);
    }
}

void Core::render()
{
    mMainWindow.clear(sf::Color(10, 10, 10));

    mSimulator.render();
    // End of the current frame
    mMainWindow.display();
}
