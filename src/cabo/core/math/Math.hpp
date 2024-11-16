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

} // namespace cn::core::math
