#include "server/game/Discard.hpp"

#include "shared/game/Constants.hpp"

#include "core/Assert.hpp"

namespace cn::server::game
{

Discard::Discard()
{
    m_cards.reserve(shared::game::StandartDeckSize);
}

void Discard::discard(Card* _card)
{
    m_cards.push_back(_card);
}

Card* Discard::getLast()
{
    CN_ASSERT(!m_cards.empty());
    auto card = m_cards.back();
    m_cards.pop_back();
    return card;
}

} // namespace cn::server::game
