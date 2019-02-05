//
// Trail.hpp
// N-Body-Simulator
//

#ifndef TRAIL_HPP
#define TRAIL_HPP

#include <memory>
#include <deque>

#include <SFML/Graphics.hpp>

#include "Effect.hpp"

namespace gfx
{



    class Trail : public Effect
    {
    public:

        typedef std::unique_ptr<Trail> Ptr;

    public:

    	Trail();

        // Methods
        void    pushNewPoint(const sf::Vector2f& point);

        void    clear();

        // Setters
        void    setMaxPoints(const unsigned& pts);



    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:

        size_t                      mMaxPoints;
        float                       mThickness;

        std::deque<sf::Vector2f>    mPoints;
        sf::VertexArray             mVertices;

    };



}

#endif // TRAIL_HPP
