//
// Celestial/Body.hpp
// N-Body-Simulator
//

#ifndef CELESTIAL_BODY_HPP
#define CELESTIAL_BODY_HPP

#include <cmath>

#include <SFML/Graphics.hpp>

#include "../Constant.hpp"
#include "../Utils.hpp"



namespace Celestial
{

    class Body : public sf::Transformable, public sf::Drawable
    {
    public:
        // Constructor
        Body(double rx, double ry, double vx, double vy, double mass, double density = 1);
        Body(sf::Vector2d pos, sf::Vector2d vel, double mass, double density = 1);

        // Methods
        void    update(sf::Time dt);

        double  distanceTo(const Body& body) const;
        void    addForce(const Body& body);
        Body    fuseWith(Body& otherBody);
        bool    hasCollidedWith(const Body& body) const;
        void    resetForce();
        void    updateVelocity(const Body& b, sf::Time dt);

        // Getters
        double          getMass() const;
        float           getRadius() const;
        sf::Vector2d    getVelocity() const;

        // Setters

        void setColor(const sf::Color& color);


    protected:
        // Internal handling
        void    calculateRadius();

    private:
        // Draw
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    public:
        // Operator Overload
        bool   operator==(const Body& rhs);
	    bool   operator!=(const Body& rhs);
        Body   operator+(const Body& rhs);

    private:

        double  mMass; // masses are in "earth" measure
        double  mDensity;
        double  mRadius;

        sf::Vector2d mVelocity; // velocity vector
        sf::Vector2d mForce; // force vector

        sf::CircleShape mBody;


    };



}

#endif // CELESTIAL_BODY_HPP
