#pragma once
#include <SFML/System/Time.hpp>

namespace cn
{
    inline constexpr unsigned UPDATES_PER_SEC = 60;
    inline constexpr float FRAME_TIME_s = 1.f / UPDATES_PER_SEC;
    const sf::Time TimePerFrame = sf::seconds(cn::FRAME_TIME_s);
} // namespace cn
