#include "shared/game/Deck.hpp"

#include "core/Assert.hpp"
#include "core/Log.hpp"

#include <numeric>

namespace cn::shared::game
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
#ifndef CN_DISABLE_LOGGING
    uint8_t arr[5];
    for (unsigned i = 0; i < 5; ++i)
        arr[i] = m_cards[i]->getId().value();
    CN_LOG_FRM("Cards {},{},{},{},{}", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
#endif
}

Card* Deck::getNextCard()
{
    CN_ASSERT(!m_cards.empty());
    Card* card = m_cards.back();
    m_cards.pop_back(); // TODO can I do this without popping

    CN_LOG_FRM("Get Next Card {}", card->getId().value());

    return card;
}

void Deck::visit(std::function<void(const Card&)> _visitor) const
{
    for (auto* card : m_cards)
        _visitor(*card);
}

} // namespace cn::shared::game
