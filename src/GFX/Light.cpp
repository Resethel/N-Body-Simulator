//
// Created by Raphaël OLLANDO on 10/11/2022.
//

#include "Light.hpp"

namespace gfx
{

    ////////// Constructors

    Light::Light()
    : Effect()
    {
        // initial parameter
        mBody.setPrimitiveType(sf::TriangleFan);
    }

    Light::~Light()
    {
        mLinkedBody.reset();
    }

////////// Methods

    void Light::linkTo(const Celestial::Body::Ptr& body)
    {
        mLinkedBody = body;
    }

    void Light::update()
    {
        if (auto linkedBodyPtr = mLinkedBody.lock())
        {

            mBody.clear();
            sf::Vertex vertex;
            sf::Color color(mColor);

            // Update the light only if the body is a star
            auto bodyType = linkedBodyPtr->getType();
            if (bodyType == Celestial::Body::SMALL_STAR
                or bodyType == Celestial::Body::MEDIUM_STAR
                or bodyType == Celestial::Body::BIG_STAR)
            {

                // update the position
                this->setPosition(linkedBodyPtr->getPosition());


                double deltaAngle;
                double radius;

                // Center
                color.a = CONSTANT::EXPLOSION_LIGHT_STRENGTH;

                vertex.position = sf::Vector2f(getX(), getY());
                vertex.color = color;

                mBody.append(vertex);

                // Global Light
                //// Calculate the deltaAngle and the radius
                deltaAngle = 2 * M_PI / ((double) CONSTANT::EXPLOSION_POINT_COUNT);
                radius = 1000;//LIGHT_STRENGTH_MULTIPLIER * sqrt(p.fusionEnergy());

                //// Outer Bound
                color.a = 0;
                for (size_t i(0); i < CONSTANT::EXPLOSION_POINT_COUNT + 1; ++i) {
                    vertex.position = sf::Vector2f(getX() + radius * cos(deltaAngle * i),
                                                   getY() + radius * sin(deltaAngle * i));
                    vertex.color = color;
                    mBody.append(vertex);
                }
            }

        }
        else if(!isDestroyed())
        {
            destroy();
        }

        Effect::update();
    }

////////// Getters

////////// Setters

////////// Draw

    void Light::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(mBody,states);
    }


}