#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <numbers>

namespace cn::core::math
{

inline float toRad(float _angleInDeg)
{
    return _angleInDeg * std::numbers::pi / 180.f;
}

inline float toDeg(float _angleInRad)
{
    return _angleInRad * 180.f / std::numbers::pi;
}

inline sf::Vector2f rotateVector(sf::Vector2f _v, float _angleDeg)
{
    _angleDeg = toRad(_angleDeg);

    float cs = std::cos(_angleDeg);
    float sn = std::sin(_angleDeg);

    return sf::Vector2f{_v.x * cs - _v.y * sn, _v.x * sn + _v.y * cs}; 
}

inline sf::Vector2f rotateVectorInverse(sf::Vector2f _v, float _angleDeg)
{
    _angleDeg = toRad(_angleDeg);

    float cs = std::cos(_angleDeg);
    float sn = std::sin(_angleDeg);

    return sf::Vector2f{_v.x * cs + _v.y * sn, - _v.x * sn + _v.y * cs}; 
}

} // namespace cn::core::math
