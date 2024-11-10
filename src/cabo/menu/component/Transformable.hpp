#pragma once

#include "menu/Types.hpp"
#include <SFML/System/Vector2.hpp>

namespace cn::menu::component
{

class Transformable
{
public:
    void setPosition(Position _position);
    sf::Vector2f calculateGlobalPos(sf::Vector2f _size, sf::Vector2u _windowSize) const;

private:
    Position m_position{};
};

} // namespace cn::menu::component