//
// Effect.hpp
// N-Body-Simulator
//

#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <SFML/Graphics.hpp>

#include "../Utils.hpp"

namespace gfx
{
    class Effect : public sf::Drawable
    {
    public:

        Effect(double x, double y, double s = 0, sf::Color col = sf::Color::White);
    	Effect();

        // Getters
        sf::Color       getColor() const;
        sf::Vector2d    getPosition() const;
        double          getSize() const;

        double          getX() const;
        double          getY() const;

        // Setters
        void    setColor(const sf::Color& c);
        void    setPosition(const double& x, const double& y);
        void    setPosition(const sf::Vector2d& p);
        void    setSize(const double& s);

    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const =0;

    protected:

        sf::Color       mColor;
        sf::Vector2d    mPosition;
        double          mSize;



    };

}

#endif // EFFECT_HPP
