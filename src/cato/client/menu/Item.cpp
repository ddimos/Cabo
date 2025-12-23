#include "client/menu/Item.hpp"

namespace cn::client::menu
{

Position Item::getPosition() const
{
    return m_transformable.getPosition();
}

void Item::setPosition(Position _position)
{
    m_transformable.setPosition(_position);
    onPositionUpdated();
}

} // namespace cn::client::menu
