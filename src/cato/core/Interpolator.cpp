#include "core/Interpolator.hpp"
#include "core/Assert.hpp"

#include <cmath>

namespace cn::core
{

float Easing::linear(float _t)
{
    return _t;
}

float Easing::easeInOutExpo(float _t)
{
    CN_ASSERT(_t >= 0.f && _t <= 1.f);
    if (_t <= 0.f)
        return 0.f;
    if (_t >= 1.f)
        return 1.f;
    if (_t < 0.5f)
        return std::pow(2.f, 20.f * _t - 10.f) * 0.5f;
    return (2.f - std::pow(2.f, -20.f * _t + 10.f)) * 0.5f;
}

float Easing::easeOutBack(float _t)
{
    CN_ASSERT(_t >= 0.f && _t <= 1.f);
    constexpr float c1 = 1.70158f;
    constexpr float c3 = c1 + 1.f;
    return 1.f + c3 * std::pow(_t - 1.f, 3.f) + c1 * std::pow(_t - 1.f, 2.f);
}

float Easing::easeInBack(float _t)
{
    CN_ASSERT(_t >= 0.f && _t <= 1.f);
    constexpr float c1 = 1.70158f;
    constexpr float c3 = c1 + 1.f;

    return c3 * _t * _t * _t - c1 * _t * _t;
}


} // namespace cn::core
