#include "game/object/Deck.hpp"

#include "core/Types.hpp"
#include "core/Assert.hpp"

#include "menu/item/Button.hpp"

#include <numeric>

namespace cn::game
{

Deck::Deck(menu::item::ButtonPtr _button, std::vector<CardPtr>&& _cards, unsigned _seed)
    : m_button(_button)
    , m_cards(std::move(_cards))
{
    m_randomizer.init(_seed);

    m_button->setPosition(cn::menu::Position{ .m_position = sf::Vector2f(850, 400) });

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

CardPtr Deck::getNextCard()
{
    CN_ASSERT(!m_cards.empty());
    CardPtr card = m_cards.back();
    m_cards.pop_back(); // can I do this without popping

    if (m_cards.empty())
        m_button->requestDeactivated();

    return card;
}

} // namespace cn::game
