//
// Celestial/Body.hpp
// N-Body-Simulator
//

#ifndef CELESTIAL_BODY_HPP
#define CELESTIAL_BODY_HPP

#include <cmath>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../Constant.hpp"
#include "../Utils.hpp"



namespace Celestial
{

    class Body : public sf::Transformable, public sf::Drawable
    {
    public:

        typedef std::shared_ptr<Body> Ptr;

    public:
        // Constructor
        Body(double rx, double ry, double vx, double vy, double mass);
        Body(sf::Vector2d pos, sf::Vector2d vel, double mass);

        // Methods
        void    update(sf::Time dt);

        double  distanceTo(const Body& body) const;
        void    addForce(const Body& body);
        Body    fuseWith(Body& otherBody);
        bool    hasCollidedWith(const Body& body) const;
        void    resetForce();
        void    updateVelocity(const Body& b, sf::Time dt);
        void    explode();

        bool   isInsideRocheLimitOf(const Body& Primary) const;
        // Getters
        double          getMass() const;
        float           getRadius() const;
        sf::Vector2d    getVelocity() const;

        // Setters

        void    setColor(const sf::Color& color);
        void    setVelocity(const double& vx, const double& vy);
        void    setVelocity(const sf::Vector2d& vel);

        // static functions

        static double rocheLimit(const Body& Primary, const Body& Secondary);


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
