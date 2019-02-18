//
// Trail.hpp
// N-Body-Simulator
//

#ifndef TRAIL_HPP
#define TRAIL_HPP

#include <cmath>
#include <deque>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../Constant.hpp"
#include "../Celestial/Body.hpp"
#include "Effect.hpp"


namespace gfx
{



    class Trail : public Effect
    {
    public:

        typedef std::unique_ptr<Trail> Ptr;

    public:

    	Trail();
        ~Trail();

        // Methods
        void            pushNewPoint(const sf::Vector2f& point);
        void            linkTo(const Celestial::Body::Ptr& body);

        virtual void    update();
        void            clear();

        // Setters
        void    setMaxPoints(const unsigned& pts);



    private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:

        size_t                      mMaxPoints;
        float                       mThickness;

        std::deque<sf::Vector2f>    mPoints;
        sf::VertexArray             mVertices;

        Celestial::Body::WeakPtr    mLinkedBody;

    };



}

#endif // TRAIL_HPP
