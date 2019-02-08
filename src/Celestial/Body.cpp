//
// Celestial/Body.cpp
// N-Body-Simulator
//

#include "Body.hpp"

#include <iostream>
namespace Celestial
{


    Body::Body(double rx, double ry, double vx, double vy, double mass)
    : mMass(mass)
    , mDensity(0.5)
    , mVelocity(vx,vy)
    , mForce(0,0)
    , mBody()
    {
        this->setPosition(rx,ry);

        calculateRadius();
        mBody.setRadius(mRadius);
        utils::centerOrigin<sf::CircleShape>(mBody);
    }

    Body::Body(sf::Vector2d pos, sf::Vector2d vel, double mass)
    : Body(pos.x,pos.y,vel.x,vel.y,mass)
    {}

////////// Methods

    void Body::update(sf::Time dt)
    {
        mVelocity.x += dt.asSeconds() * mForce.x / mMass;
        mVelocity.y += dt.asSeconds() * mForce.y / mMass;

        move(dt.asSeconds() * mVelocity.x, dt.asSeconds() * mVelocity.y);
    }

    double Body::distanceTo(const Body& body) const
    {
        double dx = getPosition().x - body.getPosition().x;
        double dy = getPosition().y - body.getPosition().y;

        return std::sqrt(dx*dx + dy*dy);
    }

    void Body::addForce(const Body& body)
    {
        double dx = body.getPosition().x - getPosition().x;
        double dy = body.getPosition().y - getPosition().y;
        double dist = std::sqrt(dx*dx + dy*dy);

        if(dist >= 10*std::numeric_limits<double>().epsilon())
        {
            double F = (CONSTANT::G * this->mMass * body.getMass()) / (dist*dist);

            mForce.x += F * dx / dist;
            mForce.y += F * dy / dist;
        }

    }

    bool Body::hasCollidedWith(const Body &body) const
    {
        return distanceTo(body) < mRadius + body.mRadius;
    }

    void Body::resetForce()
    {
        mForce = sf::Vector2d(0,0);
    }

    bool Body::isInsideRocheLimitOf(const Body& Primary) const
    {
        bool isInside(false);

        if((this->mMass < Primary.mMass)
        && (this->mMass / Primary.mMass < CONSTANT::ROCHE_LIMIT_MIN_MASS_RATIO)
        && (distanceTo(Primary) < rocheLimit(Primary, *this))
        && (this->mMass > CONSTANT::MINIMUM_MASS_FOR_DIVISION))
        {
            isInside = true;
        }

        return isInside;
    }

////////// Getters

    double Body::getMass() const
    {
        return mMass;
    }

    float Body::getRadius() const
    {
        return mRadius;
    }

    sf::Vector2d Body::getVelocity() const
    {
        return mVelocity;
    }

////////// Setters

    void Body::setColor(const sf::Color& color)
    {
        mBody.setFillColor(color);
    }

    void Body::setVelocity(const double& vx, const double& vy)
    {
        mVelocity.x = vx;
        mVelocity.y = vy;
    }

    void Body::setVelocity(const sf::Vector2d& vel)
    {
        setVelocity(vel.x,vel.y);
    }

//////// Static

    double Body::rocheLimit(const Body &Primary, const Body &Secondary)
    {
        return Secondary.mRadius * std::cbrt(2 * Primary.mMass/Secondary.mMass) * CONSTANT::ROCHE_LIMIT_MULTIPLIER;
    }

////////// Internal Handling

    void Body::calculateRadius()
    {
        // We first set the density regarding the mass:

        if (mMass < CONSTANT::ROCKY_LIMIT)
    	{
    		mDensity = 0.5;
            mBody.setFillColor(sf::Color(113,111,122));
    		mBody.setOutlineThickness(0);
    		mBody.setPointCount(30);

    	}
    	else if (mMass < CONSTANT::TERRESTIAL_LIMIT)
    	{
    		mDensity = 0.5;
            mBody.setFillColor(sf::Color(119,150,7));
    		mBody.setPointCount(40);

    	}
    	else if (mMass < CONSTANT::GAS_GIANT_LIMIT)
    	{
    		mDensity = 0.3;
            mBody.setFillColor(sf::Color(61,87,181));
    		mBody.setPointCount(50);

    	}
    	else if (mMass < CONSTANT::SMALL_STAR_LIMIT)
    	{
    		mDensity = 0.2;
            mBody.setFillColor(sf::Color(255,23,15));
    		mBody.setPointCount(90);
    	}
    	else if (mMass < CONSTANT::STAR_LIMIT)
    	{
    		mDensity = 0.15;
            mBody.setFillColor(sf::Color(255,145,15));
    		mBody.setPointCount(90);
    	}
    	else if (mMass < CONSTANT::BIG_STAR_LIMIT)
    	{
    		mDensity = 0.1;
            mBody.setFillColor(sf::Color(168,207,255));
    		mBody.setPointCount(150);
    	}
        else
        {
            mDensity = 1;
            mBody.setFillColor(sf::Color(30,30,30));
    		mBody.setPointCount(15);
        }


        // From the formula M = (4/3)*pi*R^3*density
        //mRadius = std::cbrt((3 * mMass) / (4 * M_PI * mDensity));
        mRadius = std::cbrt(mMass) / (mDensity);
    }


////////// Draw

    void Body::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= this->getTransform();
        target.draw(mBody, states);
    }



/////////// Operator Overload

    bool Body::operator==(const Body& rhs)
    {
        return (getPosition() == rhs.getPosition()
                and mMass == rhs.mMass
                and mDensity == rhs.mDensity
                and mVelocity == rhs.mVelocity);
    }
    bool Body::operator!=(const Body& rhs)
    {
        return !(*this == rhs);
    }

    Body Body::operator+(const Body& rhs)
    {
        double total_mass = mMass + rhs.mMass;
        // We first find the contribution of each planet depending on their masses
        double ratio_1 = mMass / total_mass;
    	double ratio_2 = rhs.mMass / total_mass;

        sf::Vector2d pos_1(getPosition()), pos_2(rhs.getPosition());
        auto velocity   = ratio_1 * mVelocity + ratio_2 * rhs.mVelocity;
        auto position   = ratio_1 * pos_1     + ratio_2 * pos_2;

        return Body(position,velocity,total_mass);

    }

}
