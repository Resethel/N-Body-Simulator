//
// Celestial/Sim.cpp
// N-Body-Simulator
//

#include "Sim.hpp"
#include <iostream>

namespace Celestial
{

    Sim::Sim(sf::RenderWindow& window)
    : mLinkedWindow(&window)
    , mPlanetArray()
    , mIsRunning(false)
    , mouseHeldDown(false)
    , mTempBody(nullptr)
    , mSpeedVector()
    {}

////////// Methods

    void Sim::update(sf::Time dt)
    {
        if(isRunning())
        {
            // Collisions are calculated first
            handleCollisions();

            for (size_t i(0) ; i < mPlanetArray.size(); i++)
            {
                mPlanetArray[i].resetForce();
                //Note: 2 loops --> N^2 complexity
                for (size_t j(0) ; j < mPlanetArray.size(); j++)
                {
                    if (i != j)
                        mPlanetArray[i].addForce(mPlanetArray[j]);
                }
            }

            //Then, loop again and update the bodies using timestep dt
            for (int i = 0; i < mPlanetArray.size(); i++)
            {
                mPlanetArray[i].update(dt);
            }
        }

    }


    void Sim::handleEvent(const sf::Event& event)
    {
        if(event.type == sf::Event::MouseButtonPressed and event.mouseButton.button == sf::Mouse::Left)
        {
            if(!mouseHeldDown and sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                mouseHeldDown = true;
                auto mousePos = mLinkedWindow->mapPixelToCoords(sf::Mouse::getPosition(*mLinkedWindow));
                mTempBody = std::make_shared<Body>(mousePos.x, mousePos.y,0,0,50);
            }
        }

        if(event.type == sf::Event::MouseButtonReleased)
        {
            if(mouseHeldDown and mTempBody)
            {
                mouseHeldDown = false;
                auto mousePos = mLinkedWindow->mapPixelToCoords(sf::Mouse::getPosition(*mLinkedWindow));
                double mass = mTempBody->getMass();
                auto delta = mTempBody->getPosition() - mousePos;
                mTempBody->setVelocity(mass/100 * delta.x, mass/100 * delta.y);
                mPlanetArray.push_back(*mTempBody);
                mTempBody = nullptr;

            }

        }


        // Drawing the velocity vector for a new body
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
            and mTempBody)
	    {
            auto pos = mTempBody->getPosition();
            auto mousePos = mLinkedWindow->mapPixelToCoords(sf::Mouse::getPosition(*mLinkedWindow));
            auto delta = pos - mousePos;
            mSpeedVector[0] = sf::Vertex(pos, sf::Color::Red);
		    mSpeedVector[1] = sf::Vertex(pos + delta, sf::Color::Red);

        }


    }


    void Sim::addCelestialBody(Body &b)
    {
        mPlanetArray.push_back(std::move(b));
    }

    void Sim::removeCelestialBody(const unsigned& ind)
    {
        auto it = mPlanetArray.begin() + ind;
	    *it = std::move(mPlanetArray.back());
	    mPlanetArray.pop_back();
    }

    void Sim::populate(size_t number, int center_x, int center_y, int radius)
    {
        mPlanetArray.clear();

        mPlanetArray.emplace_back(center_x, center_y, 0, 0, 150);
        auto offset = mPlanetArray.back().getRadius() + 50;

        std::mt19937 generator;
        generator.seed(std::random_device()());
	    std::uniform_int_distribution<double> massDistribution(0.1, 5);
	    std::uniform_int_distribution<int> angleDistribution(0, 360);
	    std::uniform_int_distribution<int> radiusDistribution(0, radius - offset);

        int x, y, norm;
        float angle;
        sf::Vector2d vel;


        for (size_t i(0); i < number-1; i++)
	    {
            vel = sf::Vector2d(0,0);
            angle = angleDistribution(generator);



            x = center_x + cos(angle/(2 * M_PI)) * (offset + radiusDistribution(generator));
            y = center_y + sin(angle/(2 * M_PI)) * (offset + radiusDistribution(generator));

            norm = utils::norm<double>(x - center_x, y - center_y);

            if(norm > radius/100.f)
            {
                vel = utils::normalize<double>(x - center_x, y - center_y);
                vel = sf::Vector2d(-vel.y, vel.x);
                vel.x *= 2000/ radius * (1 - norm/radius);
                vel.y *= 2000/ radius * (1 - norm/radius);
            }

		    mPlanetArray.emplace_back(x, y, vel.x, vel.y, massDistribution(generator));
	    }
    }




    bool Sim::isRunning() const
    {
        return mIsRunning;
    }

    void Sim::run()
    {
        mIsRunning = true;
    }

    void Sim::stop()
    {
        mIsRunning = false;
    }

////////// Internal Handling

    void Sim::handleCollisions()
    {
        Body* a(nullptr);
        Body* b(nullptr);

        for(size_t first(0) ; first < mPlanetArray.size() ; first++)
        {
            a = &mPlanetArray[first];
            for(size_t second(0) ; second < mPlanetArray.size() ; second++)
            {
                if(first != second)
                {
                    b = &mPlanetArray[second];

                    // roche Limit dislocation
                    if(a->isInsideRocheLimitOf(*b))
                    {
                        explodePlanet(first);
                        break; // we exit the loop as the planet doenst really exist anymore
                    }
                    else if( (*a) != (*b) and a->hasCollidedWith(*b))
                    {
                        // Add the resulting fused Body.
                        mPlanetArray.push_back((*a) + (*b));

                        //erase the two collided planets.
                        removeCelestialBody(second);
                        removeCelestialBody(first);

                        break; // we exit the loop as we only update 1 collision
                    }

                }
            }
        }

    }

    void Sim::explodePlanet(const int& ind)
    {

        if (!mPlanetArray.empty())
	    {
    		double amount = utils::rand<int>(4, 8);
    		double mass = mPlanetArray[ind].getMass();
    		auto pos = mPlanetArray[ind].getPosition();
            auto vel = mPlanetArray[ind].getVelocity();
    		double radius = mPlanetArray[ind].getRadius();

    		double angle = 0;

    		for(size_t i(0); i < amount; i++)
    		{
                Body Q(   pos.x + 3 * cos(angle) * std::cbrt(radius)
                        , pos.y + 3 * sin(angle) * std::cbrt(radius)
                        , vel.x + std::sqrt(mass) * cos(angle) * CONSTANT::ROCHE_LIMIT_SPEED_MULTIPLIER
                        , vel.y + std::sqrt(mass) * sin(angle) * CONSTANT::ROCHE_LIMIT_SPEED_MULTIPLIER
                        , mass/amount);

                angle += 2 * M_PI / amount;
                //angle = utils::rand<int>(0,360) / (2* M_PI);
    			addCelestialBody(Q);


    		}

            removeCelestialBody(ind);
        }
    }

////////// Draw

    void Sim::render() const
    {
        for(auto& b : mPlanetArray)
        {
            mLinkedWindow->draw(b);
        }

        if(mouseHeldDown and mTempBody)
        {
            mLinkedWindow->draw(mSpeedVector, 2, sf::Lines);
            mLinkedWindow->draw(*mTempBody);
        }
    }




}
