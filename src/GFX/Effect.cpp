//
// Effect.cpp
// N-Body-Simulator
//

#include "Effect.hpp"

namespace gfx
{


////////// Constructor & Destructor

    Effect::Effect()
    : mColor(sf::Color::White)
    , mSize(0)
    {}


////////// Getters

    sf::Color Effect::getColor() const
    {
        return mColor;
    }

    sf::Vector2d Effect::getPosition() const
    {
        return mPosition;
    }

    double Effect::getSize() const
    {
        return mSize;
    }

    double Effect::getX() const
    {
        return mPosition.x;
    }

    double Effect::getY() const
    {
        return mPosition.y;
    }

////////// Setters

    void Effect::setColor(const sf::Color& c)
    {
        mColor = c;
    }

    void Effect::setPosition(const double& x, const double& y)
    {
        mPosition.x = x;
        mPosition.y = y;
    }

    void Effect::setPosition(const sf::Vector2d& p)
    {
        mPosition = p;
    }


    void Effect::setSize(const double& s)
    {
        mSize = s;
    }



}
