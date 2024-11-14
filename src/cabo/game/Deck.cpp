#include "game/Deck.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace cn::game
{

Deck::Deck(const sf::Texture& _backCardTexture, short unsigned _size, unsigned _seed)
{
    m_randomizer.init(_seed);
    m_cards.reserve(_size);

    m_backCardSprite.setTexture(_backCardTexture);
    m_backCardSprite.setPosition(800, 400);
}

void Deck::shuffle()
{
}

void Deck::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_backCardSprite);
}


} // namespace cn::game
