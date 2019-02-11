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

    Trail::~Trail()
    {
        mLinkedBody.reset();
    }

////////// Methods

    void Trail::pushNewPoint(const sf::Vector2f& point)
    {
        if (mPoints.size() == mMaxPoints)
        mPoints.pop_back();

        mPoints.push_front(point);

        sf::Vector2f pos(0,0), pos_p(0,0);
        sf::Vertex   v_left, v_right;
        double  angle;
        // Drawing the trail from the last to the first
        if(mPoints.size() > 1)
        {
            mVertices.clear();

            for(size_t i(0) ; i < mPoints.size() ; ++i)
            {
                pos = mPoints[i];
                if(i > 0)
                pos_p = mPoints[i-1];

                angle = M_PI/2.f - atan((pos.y-pos_p.y)/(pos.x-pos_p.x));

                v_left.position  = sf::Vector2f(pos.x - mThickness/2.f * cos(angle),
                                                pos.y + mThickness/2.f * sin(angle));

                v_right.position = sf::Vector2f(pos.x + mThickness/2.f * cos(angle),
                                                pos.y - mThickness/2.f * sin(angle));

                v_left.color  = getColor();
                v_right.color = getColor();

                mVertices.append(v_left);
                mVertices.append(v_right);
            }

        }

    }

    void Trail::linkTo(const Celestial::Body::Ptr& body)
    {
        mLinkedBody = body;
    }

    void Trail::update()
    {
        if(!mLinkedBody.expired())
            pushNewPoint(mLinkedBody.lock()->getPosition());
        else
            destroy();

        Effect::update();
    }

    void Trail::clear()
    {
        mPoints.clear();
        mVertices.clear();
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
