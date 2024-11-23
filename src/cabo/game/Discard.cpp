#include "game/Discard.hpp"
#include "game/Card.hpp"
#include "game/Constants.hpp"
#include "menu/item/Button.hpp"
#include "core/Assert.hpp"

namespace cn::game
{

Discard::Discard(menu::item::ButtonPtr _button)
    : m_button(_button)
{
    m_button->setPosition(cn::menu::Position{ .m_position = sf::Vector2f(650, 400) });
    m_cards.reserve(StandartDeckSize);

    m_button->deactivate();
}

void Discard::discard(CardPtr _card)
{
    if (!m_cards.empty())
        m_cards.back()->deactivate();
    else
        m_button->activate();

    _card->setPosition(sf::Vector2f(650, 400));
    m_cards.push_back(_card);
}

CardPtr Discard::getLast()
{
    CN_ASSERT(!m_cards.empty());
    auto card = m_cards.back();
    m_cards.pop_back();
    return card;
}

} // namespace cn::game
