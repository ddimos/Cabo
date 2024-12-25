#pragma once

#include "menu/Types.hpp"
#include <SFML/System/Vector2.hpp>

namespace cn::menu::component
{

class Transformable
{
public:
    Position getPosition() const;
    void setPosition(Position _position);
    sf::Vector2f calculateGlobalPos(sf::Vector2f _size) const;

private:
    Position m_position{};
};

} // namespace cn::menu::component