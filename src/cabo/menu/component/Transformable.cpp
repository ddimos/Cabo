#include "menu/component/Transformable.hpp"
#include "menu/Utils.hpp"

namespace cn::menu::component
{

void Transformable::setPosition(Position _position)
{
    m_position = _position;
}

sf::Vector2f Transformable::calculateGlobalPos(sf::Vector2f _size, sf::Vector2u _windowSize) const
{
    return calculateSpecialPosition(m_position, _size, _windowSize);
}

} // namespace cn::menu::component
