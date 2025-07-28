#include "client/game/Anchor.hpp"

namespace cn::client::game
{

Anchor::Anchor(const core::Context& _context, shared::board::Anchor& _boardAnchor)
    : shared::game::Anchor(_boardAnchor)
{
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineColor(sf::Color::Red);
    m_shape.setOutlineThickness(5);
    m_shape.setRadius(_boardAnchor.getRadius());
    m_shape.setPosition(_boardAnchor.getPosition());
}

void Anchor::onUpdate(sf::Time _dt)
{
}

void Anchor::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_shape);
}

} // namespace cn::client::game
