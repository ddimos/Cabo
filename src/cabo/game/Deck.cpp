#include "game/Deck.hpp"

#include "core/Types.hpp"
#include "core/Assert.hpp"

#include "menu/item/Button.hpp"

namespace cn::game
{

Deck::Deck(menu::item::ButtonPtr _button, std::vector<CardPtr>&& _cards, unsigned _seed)
    : m_button(_button)
    , m_cards(_cards)
{
    m_randomizer.init(_seed);

    m_button->setPosition(cn::menu::Position{ .m_position = sf::Vector2f(850, 400) });
}

void Deck::shuffle()
{
}

CardPtr Deck::getNextCard()
{
    CN_ASSERT(!m_cards.empty());
    CardPtr card = m_cards.back();
    m_cards.pop_back(); // can I do this without popping

    return card;
}

} // namespace cn::game
