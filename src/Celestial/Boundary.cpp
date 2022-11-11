//
// Created by Raphaël OLLANDO on 10/11/2022.
//

#include "Boundary.hpp"

namespace Celestial
{

////////// Constructor
    Boundary::Boundary(double x, double y, double radius)
    {
        this->setPosition(x, y);
        this->setRadius(radius);
        this->mBody.setFillColor(sf::Color::Transparent);
        this->mBody.setOutlineColor(sf::Color(255,0,0,126));
        this->mBody.setOutlineThickness(20);
        this->mBody.setPointCount(150);
    }

    Boundary::Boundary(sf::Vector2d pos, double radius) : Boundary(pos.x, pos.y, radius)
    {}

////////// Methods

    bool Boundary::isWithin(const double &x, const double &y) const
    {
        if (sqrt((x - this->getPosition().x) * (x - this->getPosition().x) +
                 (y - this->getPosition().y) * (y - this->getPosition().y)) > this->mBody.getRadius())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool Boundary::isWithin(const sf::Vector2d& position) const {return isWithin(position.x, position.y);}
    bool Boundary::isWithin(const Celestial::Body& body) const {return isWithin(body.getPosition().x, body.getPosition().y);}

    void Boundary::setRadius(const double &radius)
    {
        mBody.setRadius(radius);
        mBody.setOrigin(radius, radius);
    }

////////// Draw

    void Boundary::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= this->getTransform();
        target.draw(mBody, states);
    }

} // Celestial