//
// Celestial/Body.cpp
// N-Body-Simulator
//

#include "Body.hpp"

#include <iostream>
namespace Celestial
{


    Body::Body(double rx, double ry, double vx, double vy, double mass, double density)
    : mMass(mass)
    , mDensity(density)
    , mVelocity(vx,vy)
    , mForce(0,0)
    , mBody()
    {
        this->setPosition(rx,ry);

        calculateRadius();
        mBody.setFillColor(sf::Color::Yellow);
        mBody.setRadius(mRadius);
        utils::centerOrigin<sf::CircleShape>(mBody);


    }

    Body::Body(sf::Vector2d pos, sf::Vector2d vel, double mass, double density)
    : Body(pos.x,pos.y,vel.x,vel.y,mass,density)
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
        double EPS = 1; // softening parameter (just to avoid infinities)
        double dx = body.getPosition().x - getPosition().x;
        double dy = body.getPosition().y - getPosition().y;
        double dist = std::sqrt(dx*dx + dy*dy);
        double F = (CONSTANT::G * this->mMass * body.getMass()) / (dist*dist + EPS*EPS);

        mForce.x += F * dx / dist;
        mForce.y += F * dy / dist;

    }

    bool Body::hasCollidedWith(const Body &body) const
    {
        return distanceTo(body) < mRadius + body.mRadius;
    }

    void Body::resetForce()
    {
        mForce = sf::Vector2d(0,0);
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

////////// Internal Handling

    void Body::calculateRadius()
    {
        // From the formula M = (4/3)*pi*R^3*density
        mRadius = std::cbrt((3 * mMass) / (4 * M_PI * mDensity));
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
        return (getPosition() == rhs.getPosition() and
                mMass == rhs.mMass and
                mDensity == rhs.mDensity and
                mVelocity == rhs.mVelocity);
    }
    bool Body::operator!=(const Body& rhs)
    {
        return !(*this == rhs);
    }

    Body Body::operator+(const Body& rhs)
    {

        double total_mass = mMass + rhs.mMass;
        // We first find the contribution of each planet depending on their masses
        double ratio_1 = mMass / (total_mass);
    	double ratio_2 = 1 - ratio_1;

        sf::Vector2d pos1(getPosition()), pos2(rhs.getPosition());
        auto velocity   = ratio_1 * mVelocity + ratio_2 * rhs.mVelocity;
        auto density    = ratio_1 * mDensity  + ratio_2 * rhs.mDensity;
        auto position   = ratio_1 * pos1      + ratio_2 * pos2;

        return Body(position,velocity,total_mass,density);

    }

}
