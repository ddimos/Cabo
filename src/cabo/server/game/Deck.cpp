#include "server/game/Deck.hpp"

#include "core/Assert.hpp"

#include <numeric>

namespace cn::server::game
{

Deck::Deck(std::vector<Card*>&& _cards, unsigned _seed)
    : m_cards(std::move(_cards))
{
    m_randomizer.init(_seed);
    shuffle();
}

void Deck::shuffle()
{
    for (int i = m_cards.size() - 1; i > 0; --i)
    {
        int j = m_randomizer.rand(0, i + 1);
        std::swap(m_cards[j], m_cards[i]);
    }
}

Card* Deck::getNextCard()
{
    CN_ASSERT(!m_cards.empty());
    Card* card = m_cards.back();
    m_cards.pop_back(); // TODO can I do this without popping

    return card;
}

} // namespace cn::server::game
