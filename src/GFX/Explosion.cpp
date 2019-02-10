//
// Explosion.pp
// N-Body-Simulator
//

#include "Explosion.hpp"

namespace gfx
{


////////// Constructors

    Explosion::Explosion(double x, double y, unsigned duration, float intensity)
    : Effect(x,y,0,sf::Color::Yellow)
    , mIntensity(intensity)
    , mDuration(duration)
    {
        // initial parameter
        mBody.setPrimitiveType(sf::TriangleFan);
    }

    Explosion::Explosion(sf::Vector2f pos, unsigned duration, float intensity)
    : Explosion(pos.x, pos.y, duration, intensity)
    {}


////////// Methods

    void Explosion::update()
    {
        if((!mFinished) and (mStep < mDuration))
        {
            mBody.clear();
            sf::Vertex vertex;
            sf::Color color(mColor);

            double deltaAngle(0);
            double radius(0);

            // center
            color.a = CONSTANT::EXPLOSION_LIGHT_STRENGTH;

            vertex.position = sf::Vector2f(getX(), getY());
            vertex.color    = color;

            mBody.append(vertex);

            // Outer Bound
            color.a     = 0;
            radius      = CONSTANT::EXPLOSION_FLASH_SIZE * mIntensity * (1 + mStep/static_cast<float>(mDuration));
            deltaAngle  = 2 * M_PI / CONSTANT::EXPLOSION_POINT_COUNT;


            for(size_t i(0) ; i < CONSTANT::EXPLOSION_POINT_COUNT + 1 ; ++i)
            {
                vertex.position = sf::Vector2f(getX() + radius * cos(deltaAngle * i),
                                               getY() + radius * sin(deltaAngle * i));
                vertex.color = color;
                mBody.append(vertex);
            }

        }
        else if(!mFinished)
        {
            mFinished = true;
        }

        Effect::update();



    }

    bool Explosion::isFinished() const
    {
        return mFinished;
    }

////////// Getters

    float Explosion::getIntensity() const
    {
        return mIntensity;
    }

////////// Setters

    void Explosion::setDuration(const unsigned& d)
    {}

    void Explosion::setIntensity(const float& d)
    {}

////////// Draw

    void Explosion::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(mBody,states);
    }


}
