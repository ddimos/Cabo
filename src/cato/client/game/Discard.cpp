#include "client/game/Discard.hpp"

namespace cn::client::game
{

Discard::Discard(shared::game::object::Id& _id)
    : shared::game::object::Discard(_id)
{
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineColor(sf::Color::Magenta);
    m_shape.setOutlineThickness(5);
    m_shape.setPosition(getPosition());
    m_shape.setSize(getSize());
    m_shape.setOrigin(m_shape.getSize() / 2.f);
}

void Discard::onActivate()
{
    m_shape.setPosition(getPosition());
    m_shape.setSize(getSize());
    m_shape.setOrigin(m_shape.getSize() / 2.f);
}

void Discard::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_shape);
}

} // namespace cn::client::game
