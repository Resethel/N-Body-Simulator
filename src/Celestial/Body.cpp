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

    Body::Body(const Body& body)
    : sf::Transformable(body)
    , mMass(body.mMass)
    , mDensity(body.mDensity)
    , mRadius(body.mRadius)
    , mVelocity(body.mVelocity)
    , mForce(body.mForce)
    , mBody(body.mBody)
    {}

////////// Methods

    void Body::update(sf::Time dt)
    {
        mVelocity.x += dt.asSeconds() * mForce.x / mMass;
        mVelocity.y += dt.asSeconds() * mForce.y / mMass;

        move(dt.asSeconds() * mVelocity.x, dt.asSeconds() * mVelocity.y);
    }


    void Body::addForce(const Body& body)
    {
        double dx = body.getPosition().x - getPosition().x;
        double dy = body.getPosition().y - getPosition().y;
        double angle = atan2(dy, dx);
        double dist = std::sqrt(dx*dx + dy*dy);
        double F;
        if(dist >= CONSTANT::MIN_DISTANCE_FOR_CALCULATION)
        {
            F = (CONSTANT::G * this->mMass * body.getMass()) / (dist*dist);
            mForce.x += F * cos(angle);
            mForce.y += F * sin(angle);
        }
        else
        {
            F = 1 / std::sqrt(dist*dist + CONSTANT::SOFTENING_FACTOR * CONSTANT::SOFTENING_FACTOR);
            mForce.x += F * cos(angle);
            mForce.y += F * sin(angle);
        }

        if(F > mForceStrongestAttractor)
        {
            mForceStrongestAttractor = F;
            mStrongestAttractorPosition = body.getPosition();
        }

    }

    void Body::resetForce()
    {
        mForce = sf::Vector2d(0,0);
        mForceStrongestAttractor = 0;
    }

    void Body::absorbs(const Body& b)
    {
        double total_mass   = mMass + b.mMass;
        double ratio_1      = mMass / total_mass;
    	double ratio_2      = b.mMass / total_mass;

        sf::Vector2d pos_1(getPosition()), pos_2(b.getPosition());
        auto velocity   = ratio_1 * mVelocity + ratio_2 * b.mVelocity;

        mMass = total_mass;
        mVelocity = velocity;

        this->calculateRadius();
        mBody.setRadius(static_cast<float>(mRadius));
        utils::centerOrigin<sf::CircleShape>(mBody);
    }

    double Body::distanceTo(const Body& body) const
    {
        double dx = getPosition().x - body.getPosition().x;
        double dy = getPosition().y - body.getPosition().y;

        return std::sqrt(dx*dx + dy*dy);
    }


    bool Body::hasCollidedWith(const Body &body) const
    {
        return distanceTo(body) < mRadius + body.mRadius;
    }



    bool Body::isInsideRocheLimitOf(const Body& Primary) const
    {
        bool isInside(false);

        if((this->mMass < Primary.mMass)
            and (this->mMass / Primary.mMass < CONSTANT::ROCHE_LIMIT_MIN_MASS_RATIO)
            and (distanceTo(Primary) < rocheLimit(Primary, *this))
            and (this->mMass > CONSTANT::MINIMUM_MASS_FOR_DIVISION))
        {
            isInside = true;
        }

        return isInside;
    }

////////// Getters

    sf::Color Body::getColor() const
    {
        return mBody.getFillColor();
    }

    double Body::getDensity() const
    {
        return mDensity;
    }

    double Body::getMass() const
    {
        return mMass;
    }

    double Body::getRadius() const
    {
        return mRadius;
    }

    sf::Vector2f Body::getStrongestAttractorPosition() const
    {
        return mStrongestAttractorPosition;
    }

    sf::Vector2d Body::getVelocity() const
    {
        return mVelocity;
    }

    Body::Type Body::getType() const
    {
        Type bType = UNDEFINED;

        if (mMass < CONSTANT::ROCKY_LIMIT)
            bType = ROCKY;

        else if (mMass <= CONSTANT::GAS_GIANT_LIMIT)
            bType = GAS_GIANT;

        else if (mMass <= CONSTANT::SMALL_STAR_LIMIT)
            bType = SMALL_STAR;

        else if (mMass <= CONSTANT::STAR_LIMIT)
            bType = MEDIUM_STAR;

        else if (mMass <= CONSTANT::BIG_STAR_LIMIT)
            bType = BIG_STAR;

        else
            bType = BLACK_HOLE;

        return bType;
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

        switch (this->getType())
    	{
            case ROCKY:
                mDensity = 0.5;
                mBody.setFillColor(sf::Color(113,111,122));
                mBody.setOutlineThickness(0);
                mBody.setPointCount(30);
            break;

            case TERRESTIAL:
                mDensity = 0.5;
                mBody.setFillColor(sf::Color(119,150,7));
                mBody.setPointCount(40);
            break;

            case GAS_GIANT:
                mDensity = 0.3;
                mBody.setFillColor(sf::Color(61,87,181));
                mBody.setPointCount(50);
            break;

            case SMALL_STAR:
                mDensity = 0.2;
                mBody.setFillColor(sf::Color(255,23,15));
                mBody.setPointCount(90);
            break;

            case MEDIUM_STAR:
                mDensity = 0.15;
                mBody.setFillColor(sf::Color(255,145,15));
                mBody.setPointCount(90);
            break;

            case BIG_STAR:
                mDensity = 0.1;
                mBody.setFillColor(sf::Color(168,207,255));
                mBody.setPointCount(150);
            break;

            case BLACK_HOLE:
                mDensity = INFINITY;
                mBody.setFillColor(sf::Color(30,30,30));
                mBody.setOutlineColor(sf::Color(255,255,255));
                mBody.setOutlineThickness(mRadius/10.f);
                mBody.setPointCount(25);
            break;

            default: break;
        }


        if (mMass < CONSTANT::BIG_STAR_LIMIT)
        {
            // From the formula M = (4/3)*pi*R^3*density
            //mRadius = std::cbrt((3 * mMass) / (4 * M_PI * mDensity));
            mRadius = std::cbrt(mMass) / (mDensity);
        }
        else // BlackHole = Schwarzschild radius
        {
            mRadius = 2 * mMass * (CONSTANT::G) / (CONSTANT::SPEED_OF_LIGHT * CONSTANT::SPEED_OF_LIGHT);
            mRadius *= 3e16;
            mRadius *= CONSTANT::SCHWARZSCHILD_MULTIPLIER; // Add a multiplier to keep the aspect correct
        }
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

        return {position,velocity,total_mass};

    }

}
