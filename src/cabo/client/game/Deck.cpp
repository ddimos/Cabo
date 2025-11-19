#include "client/game/Deck.hpp"

namespace cn::client::game
{

Deck::Deck(shared::game::object::Id _id, unsigned _seed)
    : shared::game::object::Deck(_id, _seed)
{
    m_shape.setFillColor(sf::Color::Transparent);
    m_shape.setOutlineColor(sf::Color::Yellow);
    m_shape.setOutlineThickness(5);
    m_shape.setPosition(getPosition());
    m_shape.setSize(getSize());
    m_shape.setOrigin(m_shape.getSize() / 2.f);
}

void Deck::onActivate()
{
    m_shape.setPosition(getPosition());
    m_shape.setSize(getSize());
    m_shape.setOrigin(m_shape.getSize() / 2.f);
}

void Deck::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_shape);
}

} // namespace cn::client::game
