#pragma once

#include "menu/Types.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace cn::menu
{

float calculateCenter(float _parentSize, float _itemSize);
float calculateSpecialPosition(Position::Special _special, float _position, float _itemSize, float _parentPosition, float _parentSize);
sf::Vector2f calculateSpecialPosition(Position _position, sf::Vector2f _size, sf::FloatRect _parentRect);
sf::Vector2f calculateSpecialPosition(Position _position, sf::Vector2f _size, sf::Vector2u _windowSize);

} // namespace cn::menu
