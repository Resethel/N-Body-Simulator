//
// Utils.hpp
// N-Body-Simulator
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <cmath>
#include <random>

#include <SFML/Graphics.hpp>

namespace sf
{
    typedef Vector2<double> Vector2d;
}

namespace utils
{

    static std::mt19937 rng;

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

   template <typename T>
   inline T distance(sf::Vector2<T> A, sf::Vector2<T> B)
   {
       return std::sqrt(pow(B.x - A.x, 2) + pow(B.y - A.y,2));
   }

   template <typename T>
   inline T distance(T Ax, T Ay, T Bx, T By)
   {
       return std::sqrt(pow(Bx - Ax, 2) + pow(By - Ay,2));
   }

   template <typename T>
   inline T normalize(const T& vector)
   {
       auto norm = std::sqrt(vector.x * vector.x + vector.y * vector.y);
       return T(vector.x/norm, vector.y/norm);
   }

   template <typename T>
   inline sf::Vector2<T> normalize(const T& vec_x, const T& vec_y)
   {
       auto norm = std::sqrt(vec_x * vec_x+ vec_y * vec_y);
       return sf::Vector2<T>(vec_x/norm, vec_y/norm);
   }

   template <typename T>
   inline T norm(sf::Vector2<T>& vec)
   {
       return std::sqrt(vec.x * vec.x+ vec.y * vec.y);
   }

   template <typename T>
   inline T norm(const T& vec_x, const T& vec_y)
   {
       return std::sqrt(vec_x * vec_x + vec_y * vec_y);
   }

   template <typename T>
   inline T rand(T min, T max)
   {
       std::uniform_int_distribution<T> uni_dist(min, max);
       return uni_dist(rng);
   }


} // utils

#endif // UTILS_HPP
