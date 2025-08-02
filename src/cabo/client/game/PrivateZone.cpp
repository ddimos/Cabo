#include "client/game/PrivateZone.hpp"

namespace cn::client::game
{

PrivateZone::PrivateZone(shared::board::PrivateZone& _boardPrivateZone)
    : shared::game::PrivateZone(_boardPrivateZone)
{
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineColor(sf::Color::Red);
    m_shape.setOutlineThickness(5);
    m_shape.setPosition(_boardPrivateZone.getPosition());
    // m_shape.setRotation(_boardPrivateZone.getRotation()); TODO
    m_shape.setSize(_boardPrivateZone.getSize());
    m_shape.setOrigin(m_shape.getSize() / 2.f);
}

void PrivateZone::onUpdate(sf::Time _dt)
{
    m_shape.setPosition(getBoardObject().getPosition());
}

void PrivateZone::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_shape);
}

} // namespace cn::client::game
