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

    if(!mFont.loadFromFile("media/Sansation.ttf"))
        throw std::runtime_error("unable to load mFont");

    mStats.setFont(mFont);
    mStats.setCharacterSize(24);
    mStats.setString("Step:\nTimestep:\nTotal mass:\nBodies:\nZoom:");
    mStats.setPosition(15,10);

    mControls = mStats;
    mControls.setPosition(15, 10 + mStats.getGlobalBounds().height + 50);
    std::string str;

    str = "Controls:\n";
    str += "Toggle Simulation (Space)\n";
    str += "Reset (R)\n";
    str += "Increase Time Multiplier (Up)\n";
    str += "Decrease Time Multiplier (Down)\n";
    str += "Add Celestial Body (Hold Shift + Click & Drag)\n";
    str += "Move view (Hold CTRL + Click & Drag)\n";
    str += "Zoom (Scroll Mouse)\n";
    str += "Hide Help (H)\n";

    mControls.setString(str);
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

    // Updating the Statistics
    mStats.setString("Step: " + std::to_string(mSimulator.getSimulationStep()) + "\n");
    mStats.setString(mStats.getString() + "Timestep: " + std::to_string(mTimeStepMultiplier) + "\n" );
    mStats.setString(mStats.getString() + "Total mass: " + std::to_string(mSimulator.getTotalMass()) + "\n");
    mStats.setString(mStats.getString() + "Bodies: " + std::to_string(mSimulator.getBodyCount()) + "\n");
    mStats.setString(mStats.getString() + "Zoom: " + std::to_string(zoom));
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

                case sf::Keyboard::R:
                    mSimulator.reset();
                break;

                case sf::Keyboard::H:
                    if(mControls.getFillColor() == sf::Color::Transparent)
                        mControls.setFillColor(sf::Color::White);
                    else
                        mControls.setFillColor(sf::Color::Transparent);
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

    sf::View v;
    v = mMainWindow.getView();
    mMainWindow.setView(mMainWindow.getDefaultView());
    mMainWindow.draw(mStats);
    mMainWindow.draw(mControls);
    mMainWindow.setView(v);
    // End of the current frame
    mMainWindow.display();
}
