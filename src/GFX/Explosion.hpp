//
// Explosion.hpp
// N-Body-Simulator
//

#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include <cmath>

#include <SFML/Graphics.hpp>

#include "../Constant.hpp"
#include "../Utils.hpp"
#include "Effect.hpp"

namespace gfx
{



    class Explosion : public Effect
    {
    public:

        // Constructors
        Explosion(double x, double y, unsigned duration = 10, float intensity = 1);
        Explosion(sf::Vector2f pos, unsigned duration = 10, float intensity = 1);

        // Methods
        virtual void update();

        bool    isFinished() const;

        // Getters
        float    getIntensity() const;

        // Setters
        void    setDuration(const unsigned& d);
        void    setIntensity(const float& d);

    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:

        float       mIntensity;
        unsigned    mDuration; // in steps
        bool        mFinished;

        sf::VertexArray mBody;

    };


}

#endif // EXPLOSION_HPP
