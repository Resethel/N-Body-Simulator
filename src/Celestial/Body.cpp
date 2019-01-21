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

        mBody.setFillColor(sf::Color::Yellow);
        mBody.setRadius(10);
        utils::centerOrigin<sf::CircleShape>(mBody);
    }

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

    void Body::resetForce()
    {
        mForce = sf::Vector2d(0,0);
    }

////////// Getters

    double Body::getMass() const
    {
        return mMass;
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

}
