#include "game/Deck.hpp"
#include "core/Types.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "ResourceIds.hpp"

namespace cn::game
{

Deck::Deck(const core::Context& _context, short unsigned _size, unsigned _seed)
{
    m_randomizer.init(_seed);
    m_cards.reserve(_size);

    m_backCardSprite.setTexture(_context.textureHolderRef.get(TextureIds::CardBacks));
    m_backCardSprite.setScale(0.75, 0.75);
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
