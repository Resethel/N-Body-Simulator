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
    , mBoundActive(false)
    , mBoundary(nullptr)
    {
        mPlanetArray.reserve(2000);
    }

////////// Methods
//////////////////// for Simulation


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
                mouseHeldDown           = false;

                // Compute the velocity
                sf::Vector2f mousePos   = mLinkedWindow->mapPixelToCoords(sf::Mouse::getPosition(*mLinkedWindow));
                sf::Vector2f delta      = (mTempBody->getPosition() - mousePos);
                mTempBody->setVelocity(mTempBody->getMass() * delta.x * CONSTANT::SLINGSHOT_SPEED_MULTIPLIER,
                                       mTempBody->getMass() * delta.y * CONSTANT::SLINGSHOT_SPEED_MULTIPLIER);

                // Add the celestial body
                addCelestialBody(*mTempBody);
                mTempBody.reset();

            }

        }


        // Drawing the velocity vector for a new body
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and mTempBody)
        {
            auto pos        = mTempBody->getPosition();
            auto mousePos   = mLinkedWindow->mapPixelToCoords(sf::Mouse::getPosition(*mLinkedWindow));
            auto delta      = (pos - mousePos) * CONSTANT::SLINGSHOT_DRAW_MULTIPLIER;
            mSpeedVector[0] = sf::Vertex(pos, sf::Color::Red);
            mSpeedVector[1] = sf::Vertex(pos + delta , sf::Color::Red);

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

    void Sim::update(sf::Time dt)
    {
        if(isRunning() and !mPlanetArray.empty())
        {
            // Update the boundary
            if (mBoundActive)
            {
                auto cm = this->getCenterOfMass();
                mBoundary->setPosition(cm.x, cm.y);
                mBoundary->setRadius(CONSTANT::DEFAULT_BOUNDARY_RADIUS); // TODO: Replace by dynamic bound
            }

            // Handling collisions, roche limit and force update
            std::thread phy_res_thread(&Sim::physicalResolution, this);

            // Handling effects
            std::thread eff_res_thread(&Sim::effectsResolution, this);

            Body::WeakPtr body;
            for (size_t i(0) ; i < mPlanetArray.size(); ++i)
            {
                body = mPlanetArray[i];
                if(auto bodyLock = body.lock())
                    bodyLock->update(dt);
            }

            //joining back the threads
            phy_res_thread.join();
            eff_res_thread.join();

            //Updating the bodies using timestep dt



            // incrementing the simulation step
            ++mSimulationStep;

        }
    }

    void Sim::render() const
    {
        for(auto& t : mTrailArray)
            mLinkedWindow->draw(t);

        for(auto& b : mPlanetArray)
        {
            mLinkedWindow->draw(*b);
        }

        // 1st: Draw the light of the stars
        for(auto& light : mLightArray)
        {
            mLinkedWindow->draw(light);
        }

        // drawing explosions
        for(auto& expl : mExplosionArray)
        {
            mLinkedWindow->draw(expl);
        }

        // Drawing the Boundary
        if(mBoundActive)
        {
            mLinkedWindow->draw(*mBoundary);
        }

        if(mouseHeldDown and mTempBody)
        {
            mLinkedWindow->draw(mSpeedVector, 2, sf::Lines);
            mLinkedWindow->draw(*mTempBody);
        }
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
        // Deleting all bodies
        mPlanetArray.clear();
        mTempBody.reset();

        // Clearing the graphics
        mTrailArray.clear();
        mExplosionArray.clear();

        // resetting the counters
        mTotalMass = 0;
        mBodyCount = 0;
        mSimulationStep = 0;
    }

    bool Sim::isRunning() const
    {
        return mIsRunning;
    }

    std::weak_ptr<const Body> Sim::getBodyAtPosition(sf::Vector2f pos) const
    {
        std::weak_ptr<const Body> body;

        // Try to find the body;
        auto found = std::find_if(mPlanetArray.begin(), mPlanetArray.end(),
            [&](const Body::Ptr& b)
            {
                auto delta = b->getPosition() - pos;
                return utils::norm<float>(delta) < b->getRadius();
            });

        if(found != mPlanetArray.end())
            body = *found;

        return body;

    }

//////////////////// for Celestial bodies


    void Sim::addCelestialBody(const Body &b, bool update_mass)
    {
        ++mBodyCount;
        if(update_mass)
            mTotalMass += b.getMass();

        auto body = std::make_shared<Body>(b);
        mPlanetArray.push_back(body);

        // Add a light to the body
        gfx::Light bodyLight;
        bodyLight.linkTo(body);
        mLightArray.push_back(bodyLight);
    }

    void Sim::addCelestialBody(double x, double y, double vel_x, double vel_y, double mass, bool update_mass)
    {
        ++mBodyCount;
        if(update_mass)
            mTotalMass += mass;

        auto body = std::make_shared<Body>(x, y, vel_x, vel_y, mass);
        mPlanetArray.emplace_back(body);

        // Add a light to the body
        gfx::Light bodyLight;
        bodyLight.linkTo(body);
        mLightArray.push_back(bodyLight);
    }

    void Sim::removeCelestialBody(const size_t& ind, bool update_mass)
    {
        auto it = mPlanetArray.begin() + ind;

        --mBodyCount;
        if(update_mass)
            mTotalMass -= (*it)->getMass();

	    *it = std::move(mPlanetArray.back());
	    mPlanetArray.pop_back();
    }

    void Sim::populate(size_t number, double center_x, double center_y, int radius)
    {
        mPlanetArray.clear();

        addCelestialBody(center_x, center_y, 0, 0, 150);
        auto offset = mPlanetArray.back()->getRadius() + 50;

        std::mt19937 generator;
        generator.seed(std::random_device()());
	    std::uniform_int_distribution<double> massDistribution(0.1, 10);
	    std::uniform_int_distribution<int> angleDistribution(0, 360);
	    std::uniform_int_distribution<int> radiusDistribution(0, radius - offset);

        double x, y, norm;
        double angle;
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
                vel.x *= 20000 / radius * (1 - norm/radius);
                vel.y *= 20000 / radius * (1 - norm/radius);
            }

		    addCelestialBody(x, y, vel.x, vel.y, massDistribution(generator));
	    }
    }

    void Sim::generateSystem(double centerX, double centerY, size_t numberOfObjects, int radius)
    {
        double systemMass = CONSTANT::MASS_MULTIPLIER * std::cbrt(radius);

        double speedMultiplier = 4.35e-7 * utils::rand(120*systemMass, 150*systemMass);

        double angle        = 0;
        double deltaAngle   = 2 * M_PI / static_cast<float>(numberOfObjects);
        double massOfCenter = utils::rand(systemMass / 3 , systemMass / 2);
        systemMass -= massOfCenter;

        Celestial::Body centerPlanet(centerX, centerY, 0, 0, massOfCenter);
        this->addCelestialBody(centerPlanet, true);

        for (int i = 0; i < numberOfObjects; i++ , angle += deltaAngle)
        {
            double mass = utils::rand(0.6 * systemMass / static_cast<float>(numberOfObjects),
                                      1.4 * systemMass / static_cast<float>(numberOfObjects));

            double radius2          = utils::rand(2 * centerPlanet.getRadius() , (double) radius);
            double randomElement1   = utils::rand(-speedMultiplier * 0.5       , speedMultiplier * 0.5);
            double randomElement2   = utils::rand(-speedMultiplier * 0.5       , speedMultiplier * 0.5);

            double x = cos(angle) * radius2;
            double y = sin(angle) * radius2;
            double vx = 50 * (speedMultiplier * cos(angle + 1.507) + randomElement1);
            double vy = 50 * (speedMultiplier * sin(angle + 1.507) + randomElement2);

            this->addCelestialBody(centerX + x, centerY + y, vx, vy, mass);
        }

    }

    void Sim::enableBoundary(const bool& activate)
    {
        if (activate)
        {
            // ignore this case
            if (mBoundActive)
                return ;
            else
            {
                mBoundActive = true;
                mBoundary = std::make_shared<Boundary>(getCenterOfMass(),CONSTANT::DEFAULT_BOUNDARY_RADIUS);
            }
        }
        else
        {
            // ignore this case
            if (!mBoundActive)
                return ;
            else
            {
                mBoundActive = false;
                mBoundary.reset();
            }
        }
    }

//////////////////// for Graphical Effects


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

    void Sim::addStarLight(gfx::Light& light)
    {
        mLightArray.push_back(std::move(light));
    }

    void Sim::removeStarLight(const size_t& ind)
    {
        auto it = mLightArray.begin() + ind;

        *it = std::move(mLightArray.back());
        mLightArray.pop_back();
    }


////////// Getters


    unsigned Sim::getBodyCount() const
    {
        return mBodyCount;
    }

    sf::Vector2d Sim::getCenterOfMass() const
    {
        double x(0), y(0), totalMass(0);

        for (const auto& p : mPlanetArray)
        {
            double planetMass = p->getMass();

            totalMass += planetMass;
            x += planetMass * p->getPosition().x;
            y += planetMass * p->getPosition().y;
        }

        return {x / totalMass, y / totalMass};
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

        // There can be only collisions between at least 2 entities
        if(mPlanetArray.size() > 1)
        {
            for(size_t first(0) ; first < mPlanetArray.size() ; ++first)
            {
                a = mPlanetArray[first].get();
                a->resetForce();

                // First check if the planet is out of bound
                if (mBoundActive and !mBoundary->isWithin(a->getPosition().x, a->getPosition().y))
                {
                    removeCelestialBody(first);
                    continue;
                }

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
                            explodeCelestialBody(first, true);
                            break; // we exit the loop as the planet doesn't really exist anymore
                        }
                        // Check for collision
                        else if( (*a) != (*b) and a->hasCollidedWith(*b))
                        {
                            // Add an explosion at impact point
                            gfx::Explosion expl(a->getPosition());
                            if(b->getMass() < a->getMass())
                            {
                                expl.setPosition(b->getPosition());
                            }
                            addExplosion(expl);


                            double massRatio = a->getMass() / b->getMass();
                            massRatio = (massRatio > 1 )? 1/massRatio : massRatio;

                            // Resolve collisions where one body should absorb another
                            if(massRatio < CONSTANT::COLLISION_ABSORPTION_RATIO)
                            {
                                if(a->getMass() > b->getMass())
                                {
                                    a->absorbs(*b);
                                    removeCelestialBody(second,false);
                                }
                                else
                                {
                                    b->absorbs(*a);
                                    removeCelestialBody(first,false);
                                }
                            }
                            else // Merge the two bodies into a new one
                            {
                                // Create the resulting fused Body.
                                auto fusion = ((*a) + (*b));

                                // Add the fused body to the simulation
                                addCelestialBody(fusion);

                                //erase the two collided planets.
                                removeCelestialBody(second);
                                removeCelestialBody(first);
                            }


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
        // Lights
        for(size_t i(0) ; i < mLightArray.size() ; ++i )
        {
            if(mLightArray[i].isDestroyed())
            {
                removeStarLight(i);
            }
            else
            {
                mLightArray[i].update();
            }
        }

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

    void Sim::explodeCelestialBody(const int& ind,  bool rocheExplosion)
    {
        if (!mPlanetArray.empty())
	    {
    		int amount          = utils::rand<int>(4, 8);
    		double mass         = mPlanetArray[ind]->getMass();
    		sf::Vector2f pos    = mPlanetArray[ind]->getPosition();
            sf::Vector2d vel    = mPlanetArray[ind]->getVelocity();
    		double radius       = mPlanetArray[ind]->getRadius();
    		double angle        = 0;

            // Set the speed multiplier
            double speedMultiplier = rocheExplosion ? CONSTANT::EXPLOSION_ROCHE_SPEED_MULTIPLIER
                                                    : CONSTANT::EXPLOSION_PLANET_SPEED_MULTIPLIER;

            for(size_t i(0); i < amount; ++i)
    		{
                Body Q(   pos.x + 3 * cos(angle)  * std::cbrt(radius)
                        , pos.y + 3 * sin(angle)  * std::cbrt(radius)
                        , vel.x + std::sqrt(mass) * cos(angle) * speedMultiplier
                        , vel.y + std::sqrt(mass) * sin(angle) * speedMultiplier
                        , mass/amount);

                angle += 2 * M_PI / amount;

    			addCelestialBody(Q);

    		}

            removeCelestialBody(ind);
        }
    }


} // namespace Celestial
