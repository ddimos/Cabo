#include "core/math/Math.hpp"

namespace cn::core::math
{

std::vector<std::pair<sf::Vector2f, float>> generatePointOnEllipse(float _r1, float _r2, unsigned _numberOfPoints, sf::Vector2f _positionOffset)
{
    // https://stackoverflow.com/a/20510150
    //
    // dp(t) = sqrt( (r1*sin(t))^2 + (r2*cos(t))^2)
    // circ = sum(dp(t), t=0..2*Pi step 0.0001)

    // n = 20
    // nextPoint = 0
    // run = 0.0
    // for t=0..2*Pi step 0.0001
    //     if n*run/circ >= nextPoint then
    //         set point (r1*cos(t), r2*sin(t))
    //         nextPoint = nextPoint + 1
    //     next
    //     run = run + dp(t)
    // next

    if (_numberOfPoints == 0)
        return {};

    std::vector<std::pair<sf::Vector2f, float>> points;
    points.reserve(_numberOfPoints);

    const float twoPi = std::numbers::pi * 2.0;
    const float deltaTheta = 0.0001;
    const unsigned numIntegrals = static_cast<unsigned>(std::round(twoPi / deltaTheta));
    float theta = 0.0;
    float circ = 0.f;
    float dpt = 0.f;

    auto computeDpt = [](float _r1, float _r2, float _theta)
    {
        float dpt_sin = std::pow(_r1 * std::sin(_theta), 2);
        float dpt_cos = std::pow(_r2 * std::cos(_theta), 2);
        float dp = std::sqrt(dpt_sin + dpt_cos);
        return dp;
    };

    // integrate over the elipse to get the circumference
    for(unsigned i = 0; i < numIntegrals; i++)
    {
        theta += i * deltaTheta;
        dpt = computeDpt(_r1, _r2, theta);
        circ += dpt;
    }

    unsigned nextPoint = 0;
    float run = 0.0;
    // Start with an offset
    theta = std::numbers::pi / 2;

    for(unsigned i = 0; i < numIntegrals; i++)
    {
        theta += deltaTheta;
        float subIntegral = (_numberOfPoints * run) / circ;
        if(subIntegral >= nextPoint)
        {
            float x = _r1 * std::cos(theta);
            float y = _r2 * std::sin(theta);
            points.push_back({sf::Vector2f(x, y) + _positionOffset, toDeg(theta)});
            nextPoint++;
        }
        run += computeDpt(_r1, _r2, theta);
    }
    return points;
}

} // namespace cn::core::math
