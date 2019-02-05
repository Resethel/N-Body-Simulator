//
// Trail.cpp
// N-Body-Simulator
//

#include "Trail.hpp"

namespace gfx
{


////////// Constructor & Destructor

        Trail::Trail()
        : Effect()
        , mMaxPoints(200)
        , mThickness(1)
        , mPoints()
        , mVertices()

        {
            mVertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
        }

////////// Methods

        void Trail::pushNewPoint(const sf::Vector2f& point)
        {
            if (mPoints.size() == mMaxPoints)
                mPoints.pop_back();

            mPoints.push_front(point);

            sf::Vector2f pos;
            sf::Vertex   v_left, v_right;
            // Drawing the trail from the last to the first
            if(mPoints.size() > 1)
            {
                mVertices.clear();

                for(size_t i(0) ; i < mPoints.size() ; ++i)
                {
                    pos = mPoints[i];

                    v_left.position  = sf::Vector2f(pos.x - mThickness/2.f, pos.y);
                    v_right.position = sf::Vector2f(pos.x + mThickness/2.f, pos.y);

                    v_left.color  = getColor();
                    v_right.color = getColor();

                    mVertices.append(v_left);
                    mVertices.append(v_right);
                }

            }

        }

////////// Setters

        void Trail::setMaxPoints(const unsigned& pts)
        {
            mMaxPoints = pts;
        }

////////// Draw

    void Trail::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(mVertices, states);
    }



}
