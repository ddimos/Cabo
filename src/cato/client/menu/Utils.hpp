#pragma once

#include <SFML/System/Vector2.hpp>

namespace cn::client::menu
{

inline float calculateCenter(float _parentSize, float _itemSize)
{
    return _parentSize / 2.f - _itemSize / 2.f;
}

inline sf::Vector2f calculateCenter(sf::Vector2f _parentSize, sf::Vector2f _itemSize)
{
    return _parentSize / 2.f - _itemSize / 2.f;
}

} // namespace cn::client::menu
