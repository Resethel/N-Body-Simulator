//
// Created by Raphaël OLLANDO on 10/11/2022.
//

#ifndef N_BODY_SIMULATOR_LIGHT_H
#define N_BODY_SIMULATOR_LIGHT_H

#include "../Constant.hpp"
#include "../Utils.hpp"
#include "../Celestial/Body.hpp"
#include "Effect.hpp"

namespace gfx
{

    class Light : public Effect
    {
    public:

            typedef std::unique_ptr<Light> Ptr;

    public:

            // Constructors
            Light();
            ~Light();

            // Methods
            void            linkTo(const Celestial::Body::Ptr& body);
            virtual void    update();

        private:

            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:

            Celestial::Body::WeakPtr mLinkedBody;
            sf::VertexArray mBody;
    };

}


#endif //N_BODY_SIMULATOR_LIGHT_H
