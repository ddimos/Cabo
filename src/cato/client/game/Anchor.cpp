#include "client/game/Anchor.hpp"

namespace cn::client::game
{

Anchor::Anchor(const core::Context& _context, shared::game::object::Id _id, int _radius)
    : shared::game::object::Anchor(_id, _radius)
{
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineColor(sf::Color::Red);
    m_shape.setOutlineThickness(5);
    m_shape.setRadius(getRadius());
    m_shape.setPosition(getPosition());
}

void Anchor::onUpdate(sf::Time _dt)
{
}

void Anchor::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_shape);
}

} // namespace cn::client::game
