//
// Created by Raphaël OLLANDO on 10/11/2022.
//

#ifndef N_BODY_SIMULATOR_BOUNDARY_HPP
#define N_BODY_SIMULATOR_BOUNDARY_HPP

#include <cmath>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../Constant.hpp"
#include "../Utils.hpp"
#include "Body.hpp"

namespace Celestial
{

    class Boundary  : public sf::Transformable, public sf::Drawable
    {
    public:

        typedef std::shared_ptr<Boundary>       Ptr;
        typedef std::weak_ptr<Boundary>         WeakPtr;
        typedef std::weak_ptr<const Boundary>   ConstWeakPtr;

    public:

        // Constructor
        Boundary(double x, double y, double radius);
        Boundary(sf::Vector2d pos, double radius);

        // Methods
        bool        isWithin(const double& x, const double& y) const;
        bool        isWithin(const sf::Vector2d& position) const;
        bool        isWithin(const Body& body) const;

        // Getters
        inline sf::Color    getColor() const {return mBody.getOutlineColor();};

        // Setters

        inline void setColor(const sf::Color& color)                {mBody.setOutlineColor(color);};
        void        setRadius(const double& radius);

    private:
        // Draw
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:

        sf::CircleShape mBody;

    };

} // Celestial

#endif //N_BODY_SIMULATOR_BOUNDARY_HPP
