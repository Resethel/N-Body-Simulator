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

    mOverlay = mStats;
    mOverlay.setString("");
    mOverlay.setFillColor(sf::Color::Transparent);

    SAline.setPrimitiveType(sf::Lines);
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


//////////// Internal Handling

void Core::update(sf::Time dt)
{
    mSimulator.update(dt);

    // Updating the Statistics
    mStats.setString("Step: " + std::to_string(mSimulator.getSimulationStep()) + "\n");
    mStats.setString(mStats.getString() + "Timestep: " + std::to_string(mTimeStepMultiplier) + "\n" );
    mStats.setString(mStats.getString() + "Total mass: " + std::to_string(mSimulator.getTotalMass()) + "\n");
    mStats.setString(mStats.getString() + "Bodies: " + std::to_string(mSimulator.getBodyCount()) + "\n");
    mStats.setString(mStats.getString() + "Zoom: " + std::to_string(zoom));

    // Updating overlay

    SAline.clear();

    if (!selectedBody.expired())
    {
        auto body = selectedBody.lock();
        std::string str;

        mOverlay.setPosition(body->getPosition() + sf::Vector2f(body->getRadius(),-body->getRadius()));
        str += "Mass: " + std::to_string(body->getMass()) + "\n";
        str += "Velocity: " + std::to_string(utils::norm<double>(body->getVelocity())) + "\n";
        str += "x: " + std::to_string(body->getPosition().x) + " y: " + std::to_string(body->getPosition().x) + "\n";

        mOverlay.setString(str);
        sf::Vertex v;
        v.position = body->getPosition();
        v.color = sf::Color::Yellow;
        SAline.append(v);
        v.position = body->getStrongestAttractorPosition();
        v.color = sf::Color::Yellow;
        SAline.append(v);



    }
    else
    {
        mOverlay.setFillColor(sf::Color::Transparent);

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

                // Resets the simulation
                case sf::Keyboard::R:
                    mSimulator.reset();
                break;

                // Toggle Help Status
                case sf::Keyboard::H:
                    if(mControls.getFillColor() == sf::Color::Transparent)
                        mControls.setFillColor(sf::Color::White);
                    else
                        mControls.setFillColor(sf::Color::Transparent);
                break;

                // Toggle view lock
                case sf::Keyboard::L:
                    if(!selectedBody.expired())
                        lockedOnBody = selectedBody.lock();
                    else
                        lockedOnBody.reset();
                break;

                default:
                break;
            }

        }


        // Mouse Events

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                if( sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) or
                    sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
                {
                    draggingView = true;
                    oldMousePosition = mMainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                    // We stop the view lock
                    lockedOnBody.reset();
                }
            }
        }

        if (event.type == sf::Event::MouseButtonReleased)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                if(draggingView)
                {
                    draggingView = false;
                }
                else if (!( sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) or
                            sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) or
                            sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) or
                            sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)))
                {
                    auto mousePos =  mMainWindow.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    if(selectedBody.expired())
                    {
                        selectedBody = mSimulator.getBodyAtPosition(mousePos);
                        if(!selectedBody.expired())
                            mOverlay.setFillColor(sf::Color::White);
                    }
                    else
                    {
                        auto delta = selectedBody.lock()->getPosition() - mousePos;
                        if(utils::norm<float>(delta) > selectedBody.lock()->getRadius() )
                        {
                            selectedBody = mSimulator.getBodyAtPosition(mousePos);
                            if(selectedBody.expired())
                                mOverlay.setFillColor(sf::Color::Transparent);
                            else
                                mOverlay.setFillColor(sf::Color::White);
                        }
                    }
                }
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
                    zoom = std::min(5.f, zoom + .05f);
                else if (event.mouseWheelScroll.delta >= 1)
                    zoom = std::max(.1f, zoom - .05f);

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

    // View lock
    if(!lockedOnBody.expired())
    {
        auto v = mMainWindow.getView();
        v.setCenter(lockedOnBody.lock()->getPosition());
        mMainWindow.setView(v);
    }


    if(SAline.getVertexCount() != 0)
        mMainWindow.draw(SAline);

    mSimulator.render();
    mMainWindow.draw(mOverlay);

    sf::View v;
    v = mMainWindow.getView();
    mMainWindow.setView(mMainWindow.getDefaultView());
    mMainWindow.draw(mStats);
    mMainWindow.draw(mControls);
    mMainWindow.setView(v);
    // End of the current frame
    mMainWindow.display();
}
