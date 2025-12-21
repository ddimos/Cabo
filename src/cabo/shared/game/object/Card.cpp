#include "shared/game/object/Card.hpp"
#include "core/Assert.hpp"

namespace cn::shared::game::object
{

Card::Card(Id _id)
    : Object(_id)
    , m_attachableComponent(*this)
    , m_flippableComponent(*this)
    , m_grabbableComponent(*this)
    , m_layerableComponent(*this)
    , m_privateZoneViewableComponent(*this)
{
}

void Card::release(sf::Vector2f _newPos)
{
    onReleased(_newPos);
}

void Card::addToDeck()
{
    CN_ASSERT(!m_isInDeck);
    m_isInDeck = true;
}

void Card::removeFromDeck()
{
    CN_ASSERT(m_isInDeck);
    m_isInDeck = false;
}

void Card::addToDiscard()
{
    CN_ASSERT(!m_isInDiscard);
    m_isInDiscard = true;
}

void Card::removeFromDiscard()
{
    CN_ASSERT(m_isInDiscard);
    m_isInDiscard = false;
}

void Card::onReleased(sf::Vector2f _newPos)
{
    setPosition(_newPos);
}

} // namespace cn::shared::game::object
