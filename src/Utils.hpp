//
// Utils.hpp
// N-Body-Simulator
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <random>

#include <SFML/Graphics.hpp>

namespace sf
{
    typedef Vector2<double> Vector2d;
}

namespace utils
{

   template <typename T>
   inline T clamp(const T& n, const T& lower, const T& upper)
   {
       return std::max(lower, std::min(n, upper));
   }


   template <typename T>
   inline void centerOrigin(T& object)
   {
       auto bounds = object.getLocalBounds();
       object.setOrigin(bounds.width/2.f, bounds.height/2.f);
   }

} // utils

#endif // UTILS_HPP
