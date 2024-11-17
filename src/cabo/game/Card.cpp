#include "game/Card.hpp"

namespace cn::game
{

Card::Card(Rank _rank, Suit _suit, const sf::Texture& _texture)
{
    m_sprite.setTexture(_texture);
    m_sprite.setScale(0.4, 0.4);
}

void Card::draw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

void Card::deal(/*player id*/ /*position*/)
{
}

void Card::discard()
{
}

void Card::setPosition(sf::Vector2f _position)
{
    m_sprite.setPosition(_position);
}

void Card::setRotation(float _angleDeg)
{
    m_sprite.setRotation(_angleDeg);
}

} // namespace cn::game
