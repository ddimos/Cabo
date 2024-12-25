#include "menu/component/Transformable.hpp"
#include "menu/Utils.hpp"

namespace cn::menu::component
{

Position Transformable::getPosition() const
{
    return m_position;
}

void Transformable::setPosition(Position _position)
{
    m_position = _position;
}

sf::Vector2f Transformable::calculateGlobalPos(sf::Vector2f _size) const
{
    return m_position.calculateGlobalPos(_size);
}

} // namespace cn::menu::component
