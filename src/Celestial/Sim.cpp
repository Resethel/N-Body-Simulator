//
// Celestial/Sim.cpp
// N-Body-Simulator
//

#include "Sim.hpp"

namespace Celestial
{

    Sim::Sim(sf::RenderWindow& window)
    : mIsRunning(false)
    , mLinkedWindow(&window)
    , mPlanetArray()
    {}

////////// Methods

    void Sim::addCelestialBody(Body &b)
    {
        mPlanetArray.push_back(std::move(b));
    }

    void Sim::populate(int number, int center_x, int center_y, int radius)
    {
        std::mt19937 generator;
        generator.seed(std::random_device()());
	    std::uniform_int_distribution<int> massDistribution(10, 10000);
	    std::uniform_int_distribution<int> angleDistribution(0, 360);
	    std::uniform_int_distribution<int> radiusDistribution(0, radius);

        int x;
        int y;
        float angle;
        int norm;
        sf::Vector2d vel;


        for (unsigned i(0); i < number-1; i++)
	    {
            vel = sf::Vector2d(0,0);
            angle = angleDistribution(generator);

            x = center_x + cos(angle/(2 * M_PI)) * radiusDistribution(generator);;
            y = center_y + sin(angle/(2 * M_PI)) * radiusDistribution(generator);;

            norm = utils::norm<double>(x - center_x, y - center_y);

            if(norm > radius/100.f)
            {
                vel = utils::normalize<double>(x - center_x, y - center_y);
                vel = sf::Vector2d(-vel.y, vel.x);
                vel.x *= radius/10.f * (1 - norm/radius);
                vel.y *= radius/10.f * (1 - norm/radius);
            }

		    mPlanetArray.emplace_back(x, y, vel.x, vel.y, massDistribution(generator));
	    }

        mPlanetArray.emplace_back(center_x, center_y, 0, 0, 1000000*100, 0.25);

    }

    void Sim::update(sf::Time dt)
    {
        if(isRunning())
        {
            handleCollisions();

            for (int i = 0; i < mPlanetArray.size(); i++)
            {
                mPlanetArray[i].resetForce();
                //Notice-2 loops-->N^2 complexity
                for (int j = 0; j < mPlanetArray.size(); j++)
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
        bool collisionDetected = false;
    	auto first = mPlanetArray.begin();

    	while(first != mPlanetArray.end())
    	{
    		for(auto second = mPlanetArray.begin(); second != mPlanetArray.end(); second++)
    		{
    			if( (*first) != (*second) and first->hasCollidedWith(*second))
    			{
    				mPlanetArray.push_back((*first) + (*second));
    				mPlanetArray.erase(second);
    				first = mPlanetArray.erase(first);			//erase the two collided planets.
    				collisionDetected = true;
    				break;
    			}
    		}

            //If there is a collision, then "first" automatically points to the next value, so there is no need to increment it.
    		if(collisionDetected)
    			collisionDetected = false;
    		else
    			first++;
    	}

    }

////////// Draw

    void Sim::render() const
    {
        //sf::View previousView = mLinkedWindow->getView();
        //mLinkedWindow->setView(mLinkedWindow->getDefaultView());

        for(auto& b : mPlanetArray)
        {
            mLinkedWindow->draw(b);
        }

        //mLinkedWindow->setView(previousView);
    }




}
