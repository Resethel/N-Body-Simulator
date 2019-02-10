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

        // Methods

        virtual void update();

        bool         isDestroyed() const;

        // Getters
        sf::Color       getColor() const;
        sf::Vector2d    getPosition() const;
        double          getSize() const;
        unsigned        getStepCount() const;

        double          getX() const;
        double          getY() const;

        // Setters
        void    setColor(const sf::Color& c);
        void    setPosition(const double& x, const double& y);
        void    setPosition(const sf::Vector2d& p);
        void    setPosition(const sf::Vector2f& p);
        void    setSize(const double& s);

    protected:
        // Internal Handling
        void    destroy();


    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const =0;

    protected:

        sf::Color       mColor;
        sf::Vector2d    mPosition;
        double          mSize;
        unsigned        mStep;

        bool            mDestroyed;



    };

}

#endif // EFFECT_HPP
