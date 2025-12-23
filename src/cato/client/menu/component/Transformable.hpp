#pragma once

#include "client/menu/Types.hpp"
#include <SFML/System/Vector2.hpp>

namespace cn::client::menu::component
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

} // namespace cn::client::menu::component