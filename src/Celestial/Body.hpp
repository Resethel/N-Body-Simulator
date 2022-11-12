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
        // Ptr Definitions
        typedef std::shared_ptr<Body>       Ptr;
        typedef std::weak_ptr<Body>         WeakPtr;
        typedef std::weak_ptr<const Body>   ConstWeakPtr;

        // Celestial Body Types
        enum Type { UNDEFINED, ROCKY, TERRESTIAL, GAS_GIANT, SMALL_STAR, MEDIUM_STAR, BIG_STAR, BLACK_HOLE };

    public:
        // Constructor
        Body(double rx, double ry, double vx, double vy, double mass);
        Body(sf::Vector2d pos, sf::Vector2d vel, double mass);
        Body(const Body& body);

        // Methods
        void    update(sf::Time dt);

        void    addForce(const Body& body);
        void    resetForce();

        void    absorbs(const Body& b);

        double  distanceTo(const Body& body) const;
        bool    hasCollidedWith(const Body& body) const;
        bool    isInsideRocheLimitOf(const Body& Primary) const;

        // Getters
        sf::Color       getColor() const;
        double          getDensity() const;
        double          getMass() const;
        double          getRadius() const;
        Type            getType() const;
        sf::Vector2f    getStrongestAttractorPosition() const;
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

        double          mMass; // masses are in "earth" measure
        double          mDensity;
        double          mRadius;

        sf::Vector2d    mVelocity; // velocity vector
        sf::Vector2d    mForce; // force vector

        sf::CircleShape mBody;

        double          mForceStrongestAttractor;
        sf::Vector2f    mStrongestAttractorPosition;


    };



}

#endif // CELESTIAL_BODY_HPP
