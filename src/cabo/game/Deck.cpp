#include "game/Deck.hpp"
#include "core/Types.hpp"
#include "core/Assert.hpp"

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
    m_backCardSprite.setScale(0.4, 0.4);
    m_backCardSprite.setPosition(850, 400);

    for (short unsigned i = 0; i < _size; ++i)
    {
        m_cards.emplace_back(Card::Rank::Ace, Card::Suit::Clubs, _context.textureHolderRef.get(TextureIds::Cards));
    }
}

void Deck::shuffle()
{
}

Card Deck::getNextCard()
{
    CN_ASSERT(!m_cards.empty());
    Card card = m_cards.back();
    m_cards.pop_back(); // can I do this without popping

    return card;
}

void Deck::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_backCardSprite);
}


} // namespace cn::game
