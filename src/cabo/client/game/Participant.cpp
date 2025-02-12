#include "client/game/Participant.hpp"
#include "client/game/Card.hpp"

#include "client/menu/item/Button.hpp"
#include "client/ResourceIds.hpp"

#include "core/math/Math.hpp"
#include "core/Context.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::game
{

Participant::Participant(const core::Context& _context, PlayerId _id, bool _isLocal, std::map<ParticipantSlotId, ParticipantSlot>&& _slots, unsigned short _initialNumberOfSlots)
    : m_id(_id)
    , m_isLocal(_isLocal)
    , m_slots(std::move(_slots))
    , m_currentNumberOfSlots(_initialNumberOfSlots)
{
    auto& fontHolderRef = _context.get<client::FontHolder>();

    m_nameText.setFont(fontHolderRef.get(client::FontIds::Main));
    m_nameText.setString("Player " + std::to_string(m_id)); // TODO a proper name
    m_nameText.setFillColor(sf::Color::White);
    m_nameText.setCharacterSize(24);
    m_nameText.setOrigin(m_nameText.getGlobalBounds().getSize() / 2.f);
}

void Participant::setSpawnPoint(PlayerSpawnPoint _spawnPoint)
{
    m_spawnPoint = _spawnPoint;
    {
        sf::Vector2f localPos = sf::Vector2f(0, 15);
        localPos = core::math::rotateVector(localPos, m_spawnPoint.angleDeg + 90.f);
        m_nameText.setPosition(_spawnPoint.pos + localPos);
    }

    float offsetBetweenCardsX = 85;
    float offsetBetweenCardsY = 115;
    int cardsInRow = 4;
    int i = 0;
    int j = 0;
    for (auto& [id, slot] : m_slots)
    {
        sf::Vector2f localPos = sf::Vector2f(i * offsetBetweenCardsX - 165, j * offsetBetweenCardsY + 20);
        localPos = core::math::rotateVector(localPos, m_spawnPoint.angleDeg + 90.f);
        slot.button->setPosition(client::menu::Position{ .m_position = (m_spawnPoint.pos + localPos) });
        slot.button->setRotation(m_spawnPoint.angleDeg + 90.f);
        ++i;
        if(i >= cardsInRow)
        {
            i = 0;
            j++;
        }
    }
}

void Participant::addSlot()
{
    if (m_currentNumberOfSlots >= m_slots.size())
    {
        CN_ASSERT(false);
        return; // ParticipantSlotIdInvalid
    }

    m_currentNumberOfSlots++;
}

void Participant::removeSlot(ParticipantSlotId _id)
{
    if (m_currentNumberOfSlots == 0)
    {
        CN_ASSERT(false);
        return;
    }
    m_currentNumberOfSlots--;

    m_slots.at(_id).button->requestDeactivated();
}

void Participant::showCardInSlot(ParticipantSlotId _id)
{
    auto slot = getSlot(_id);
    slot.card->requestActivated();
    slot.button->requestDeactivated();

    // TODO think about a better position
    sf::Vector2f localPos = sf::Vector2f(50, 200);
    localPos = core::math::rotateVector(localPos, m_spawnPoint.angleDeg + 90.f);
    
    slot.card->setPosition(m_spawnPoint.pos + localPos);
}

void Participant::hideCardInSlot(ParticipantSlotId _id)
{
    auto slot = getSlot(_id);
    slot.card->requestDeactivated();
    slot.button->requestActivated();
}

const ParticipantSlot& Participant::getSlot(ParticipantSlotId _id) const
{
    CN_ASSERT(m_slots.contains(_id));
    return m_slots.at(_id);
}

Card* Participant::getCard(ParticipantSlotId _id) const
{
    return getSlot(_id).card;
}

Card* Participant::replace(ParticipantSlotId _id, Card* _card)
{
    auto prevCard = m_slots.at(_id).card;
    m_slots.at(_id).card = _card;
    return prevCard;
}

void Participant::deal(Card* _card)
{
    bool assigned = false;

    for (auto& [id, slot] : m_slots)
    {
        if (slot.card)
            continue;

        slot.card = _card;
        slot.button->requestActivated();
        assigned = true;
        break;
    }
    CN_ASSERT(assigned);
}

void Participant::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_nameText);
}

} // namespace cn::client::game