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
    , mBodyCount(0)
    , mTotalMass(0)
    , mSimulationStep(0)
    , mIsRunning(false)
    , mouseHeldDown(false)
    , mTempBody(nullptr)
    , mSpeedVector()
    , mTrailedBody(nullptr)
    {
        mPlanetArray.reserve(2000);
    }

////////// Methods

    void Sim::update(sf::Time dt)
    {
        if(isRunning() and !mPlanetArray.empty())
        {
            // Handling collisions, roche limit and force update
            std::thread phy_res_thread(&Sim::physicalResolution, this);

            // Handling effects
            std::thread eff_res_thread(&Sim::effectsResolution, this);

            //Updating the bodies using timestep dt
            for (size_t i(0) ; i < mPlanetArray.size(); ++i)
            {
                mPlanetArray[i]->update(dt);
            }

            //joining back the threads
            phy_res_thread.join();
            eff_res_thread.join();
/*
            if(mTrailedBody)
            {
                if(mPlanetArray.empty())
                {

                    mTrailedBody = nullptr;
                }
                else if(*mTrailedBody == *mPlanetArray[0])
                {
                    mTrail.pushNewPoint(mTrailedBody->getPosition());
                }
                else
                {
                    mTrail.clear();
                    mTrailedBody = mPlanetArray[0].get();
                }
            }
            else if(!mPlanetArray.empty())
            {
                mTrailedBody = mPlanetArray[0].get();
            }*/

            // incrementing the simulation step
            ++mSimulationStep;

        }



    }

    void Sim::handleEvent(const sf::Event& event)
    {
        //
        if(event.type == sf::Event::MouseButtonPressed and event.mouseButton.button == sf::Mouse::Left)
        {
            if(!mouseHeldDown and sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                mouseHeldDown   = true;
                auto mousePos   = mLinkedWindow->mapPixelToCoords(sf::Mouse::getPosition(*mLinkedWindow));
                mTempBody       = std::make_shared<Body>(mousePos.x, mousePos.y,0,0,50);
            }
        }

        //
        if(event.type == sf::Event::MouseButtonReleased)
        {
            if(mouseHeldDown and mTempBody)
            {
                mouseHeldDown   = false;
                auto mousePos   = mLinkedWindow->mapPixelToCoords(sf::Mouse::getPosition(*mLinkedWindow));
                double mass     = mTempBody->getMass();
                auto delta      = mTempBody->getPosition() - mousePos;
                mTempBody->setVelocity(mass/100 * delta.x, mass/100 * delta.y);

                addCelestialBody(*mTempBody);

                mTempBody.reset();

            }

        }


        // Drawing the velocity vector for a new body
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
            and mTempBody)
	    {
            auto pos        = mTempBody->getPosition();
            auto mousePos   = mLinkedWindow->mapPixelToCoords(sf::Mouse::getPosition(*mLinkedWindow));
            auto delta      = pos - mousePos;
            mSpeedVector[0] = sf::Vertex(pos, sf::Color::Red);
		    mSpeedVector[1] = sf::Vertex(pos + delta, sf::Color::Red);

        }

        //
        if(event.type == sf::Event::KeyReleased and event.key.code == sf::Keyboard::T)
        {
            mTrailArray.clear();

            if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                for(auto& b : mPlanetArray)
                {
                    gfx::Trail t;
                    t.linkTo(b);
                    mTrailArray.push_back(std::move(t));
                }
            }
        }
    }

    void Sim::addCelestialBody(const Body &b)
    {
        ++mBodyCount;
        mTotalMass += b.getMass();

        mPlanetArray.push_back(std::make_shared<Body>(b));
    }

    void Sim::addCelestialBody(double x, double y, double vel_x, double vel_y, double mass)
    {
        ++mBodyCount;
        mTotalMass += mass;

        mPlanetArray.emplace_back(std::make_shared<Body>(x, y, vel_x, vel_y, mass));
    }

    void Sim::removeCelestialBody(const size_t& ind)
    {
        auto it = mPlanetArray.begin() + ind;

        --mBodyCount;
        mTotalMass -= (*it)->getMass();

	    *it = std::move(mPlanetArray.back());
	    mPlanetArray.pop_back();
    }

    void Sim::populate(size_t number, int center_x, int center_y, int radius)
    {
        mPlanetArray.clear();

        addCelestialBody(center_x, center_y, 0, 0, 150);
        auto offset = mPlanetArray.back()->getRadius() + 50;

        std::mt19937 generator;
        generator.seed(std::random_device()());
	    std::uniform_int_distribution<double> massDistribution(0.1, 5);
	    std::uniform_int_distribution<int> angleDistribution(0, 360);
	    std::uniform_int_distribution<int> radiusDistribution(0, radius - offset);

        int x, y, norm;
        float angle;
        sf::Vector2d vel;


        for (size_t i(0); i < number-1; ++i)
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

		    addCelestialBody(x, y, vel.x, vel.y, massDistribution(generator));
	    }
    }

    void Sim::addExplosion(gfx::Explosion& expl)
    {
        mExplosionArray.push_back(std::move(expl));
    }

    void Sim::removeExplosion(const size_t& ind)
    {
        auto it = mExplosionArray.begin() + ind;

	    *it = std::move(mExplosionArray.back());
	    mExplosionArray.pop_back();
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

    void Sim::reset()
    {
        mPlanetArray.clear();
        mTempBody.reset();

        // resetting the counters
        mTotalMass = 0;
        mBodyCount = 0;
        mSimulationStep = 0;
    }

////////// Getters

    unsigned Sim::getBodyCount() const
    {
        return mBodyCount;
    }

    double Sim::getTotalMass() const
    {
        return mTotalMass;
    }

    unsigned long long Sim::getSimulationStep() const
    {
        return mSimulationStep;
    }

////////// Internal Handling

    void Sim::physicalResolution()
    {
        Body* a(nullptr);
        Body* b(nullptr);

        // Well, there can be only collisions between at least 2 entities
        if(mPlanetArray.size() > 1)
        {
            for(size_t first(0) ; first < mPlanetArray.size() ; ++first)
            {
                a = mPlanetArray[first].get();
                a->resetForce();

                for(size_t second(0) ; second < mPlanetArray.size() ; ++second)
                {
                    if(first != second)
                    {
                        b = mPlanetArray[second].get();

                        // Check for Roche Limit Dislocation
                        if(a->isInsideRocheLimitOf(*b))
                        {
                            gfx::Explosion expl(a->getPosition());
                            addExplosion(expl);
                            dislocateBody(first);
                            break; // we exit the loop as the planet doenst really exist anymore
                        }
                        // Check for collision
                        else if( (*a) != (*b) and a->hasCollidedWith(*b))
                        {
                            // Create the resulting fused Body.
                            auto fusion = ((*a) + (*b));

                            // Add an explosion at impact
                            gfx::Explosion expl(a->getPosition());
                            if(b->getMass() < a->getMass())
                            {
                                expl.setPosition(b->getPosition());
                            }

                            addExplosion(expl);

                            // Add the fused body to the simulation
                            addCelestialBody(fusion);

                            //erase the two collided planets.
                            removeCelestialBody(second);
                            removeCelestialBody(first);

                            break; // we exit the loop as we only update 1 collision
                        }
                        // Update forces
                        else
                        {
                            a->addForce(*b);
                        }

                    }
                }
            }
        }
    }

    void Sim::effectsResolution()
    {
        // EXPLOSIONS

        for(size_t i(0) ; i < mExplosionArray.size() ; ++i )
        {
            if(mExplosionArray[i].isDestroyed())
            {
                removeExplosion(i);
            }
            else
            {
                mExplosionArray[i].update();
            }
        }

        // Trails


        for(size_t i(0) ; i < mTrailArray.size() ; ++i)
        {
            if(mTrailArray[i].isDestroyed())
            {
                auto it = mTrailArray.begin() + i;
        	    *it = std::move(mTrailArray.back());
        	    mTrailArray.pop_back();
            }
            else
            {
                mTrailArray[i].update();
            }
        }
    }

    void Sim::dislocateBody(const int& ind)
    {

        if (!mPlanetArray.empty())
	    {
    		double amount = utils::rand<int>(4, 8);
    		double mass   = mPlanetArray[ind]->getMass();
    		auto pos      = mPlanetArray[ind]->getPosition();
            auto vel      = mPlanetArray[ind]->getVelocity();
    		double radius = mPlanetArray[ind]->getRadius();

    		double angle = 0;

    		for(size_t i(0); i < amount; ++i)
    		{
                Body Q(   pos.x + 3 * cos(angle)  * std::cbrt(radius)
                        , pos.y + 3 * sin(angle)  * std::cbrt(radius)
                        , vel.x + std::sqrt(mass) * cos(angle) * CONSTANT::ROCHE_LIMIT_SPEED_MULTIPLIER
                        , vel.y + std::sqrt(mass) * sin(angle) * CONSTANT::ROCHE_LIMIT_SPEED_MULTIPLIER
                        , mass/amount);

                angle += 2 * M_PI / amount;

    			addCelestialBody(Q);

    		}

            removeCelestialBody(ind);
        }
    }

////////// Draw

    void Sim::render() const
    {
        for(auto& t : mTrailArray)
            mLinkedWindow->draw(t);

        for(auto& b : mPlanetArray)
        {
            mLinkedWindow->draw(*b);
        }

        // drawing explosions
        for(auto& expl : mExplosionArray)
        {
            mLinkedWindow->draw(expl);
        }

        if(mouseHeldDown and mTempBody)
        {
            mLinkedWindow->draw(mSpeedVector, 2, sf::Lines);
            mLinkedWindow->draw(*mTempBody);
        }
    }



}
