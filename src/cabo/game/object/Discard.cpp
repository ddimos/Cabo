#include "game/object/Discard.hpp"
#include "game/object/Card.hpp"
#include "game/Constants.hpp"
#include "client/menu/item/Button.hpp"
#include "core/Assert.hpp"

namespace cn::game
{

Discard::Discard(client::menu::item::ButtonPtr _button)
    : m_button(_button)
{
    m_button->setPosition(client::menu::Position{ .m_position = sf::Vector2f(650, 400) });
    m_cards.reserve(StandartDeckSize);
}

void Discard::discard(CardPtr _card)
{
    if (!m_cards.empty())
        m_cards.back()->requestDeactivated();
    else
        m_button->requestActivated();

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
