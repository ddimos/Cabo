#include "client/game/Participant.hpp"
#include "client/game/SpriteSheet.hpp"

#include "client/menu/item/Button.hpp"
#include "client/ResourceIds.hpp"

#include "core/math/Math.hpp"
#include "core/Context.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::game
{

Participant::Participant(const core::Context& _context, PlayerId _id, bool _isLocal, std::vector<ParticipantSlot>&& _slots, unsigned short _initialNumberOfSlots)
    : m_id(_id)
    , m_isLocal(_isLocal)
    , m_slots(std::move(_slots))
    , m_currentNumberOfSlots(_initialNumberOfSlots)
{
    auto& fontHolderRef = _context.get<client::FontHolder>();

    m_nameText.setFont(fontHolderRef.get(client::FontIds::Main));
    m_nameText.setString("Player " + std::to_string(m_id.value())); // TODO a proper name
    m_nameText.setFillColor(sf::Color::White);
    m_nameText.setCharacterSize(24);
    m_nameText.setOrigin(m_nameText.getGlobalBounds().getSize() / 2.f);
}

void Participant::setSpawnPoint(PlayerSpawnPoint _spawnPoint)
{
    // This method can be useful in case of resizing the window
    
    m_spawnPoint = _spawnPoint;
    float rotation = m_spawnPoint.angleDeg + 90.f;

    {
        sf::Vector2f localPos = sf::Vector2f(0, 15);
        localPos = core::math::rotateVector(localPos, rotation);
        m_nameText.setPosition(_spawnPoint.pos + localPos);
    }
    {
        sf::Vector2f localPos = sf::Vector2f(0, 210.f);
        localPos = core::math::rotateVector(localPos, rotation);
        m_openCardPosition = m_spawnPoint.pos + localPos;
        m_openCardRotation = rotation + 180.f;
    }

    // TODO add to consts
    const float offsetBetweenCardsX = 85.f;
    const float offsetBetweenCardsY = 115.f;
    const int cardsInRow = 4;

    float imageRotation = rotation;
    if (m_spawnPoint.angleDeg >= 0 && m_spawnPoint.angleDeg <= 180)
        imageRotation += 180.f;

    int i = 0;
    int j = 0;
    for (auto& slot : m_slots)
    {
        sf::Vector2f localPos = sf::Vector2f(i * offsetBetweenCardsX - 127, j * offsetBetweenCardsY + 70);
        localPos = core::math::rotateVector(localPos, rotation);
        slot.position = (m_spawnPoint.pos + localPos);
        slot.rotation = imageRotation;
        slot.buttonRef.setPosition(client::menu::Position{ .m_position = (m_spawnPoint.pos + localPos) });
        slot.buttonRef.setRotation(rotation);
        ++i;
        if(i >= cardsInRow)
        {
            i = 0;
            j++;
        }
    }
}

void Participant::addSlot(ParticipantSlotId _id)
{
    if (m_currentNumberOfSlots >= m_slots.size())
    {
        CN_ASSERT(false);
        return;
    }

    m_currentNumberOfSlots++;
    auto& slot = getSlot(_id);
    CN_ASSERT(!slot.wasRemoved);
}

void Participant::removeSlot(ParticipantSlotId _id)
{
    if (m_currentNumberOfSlots == 0)
    {
        CN_ASSERT(false);
        return;
    }
    m_currentNumberOfSlots--;

    auto& slot = getSlot(_id);
    CN_ASSERT(!slot.wasRemoved);
    if (slot.buttonRef.isActivated())
        slot.buttonRef.requestDeactivated();
    
    // Or should I just erase it? 
    slot.wasRemoved = true;
}

void Participant::setCardInSlot(ParticipantSlotId _id, Card::Rank _rank, Card::Suit _suit)
{
    auto& slot = getSlot(_id);

    slot.cardPtr->set(_rank, _suit);
}

Card* Participant::replaceCardInSlot(ParticipantSlotId _id, Card* _card)
{
    // TODO it's looks identical to the one in the server part 
    auto* prevCard = getSlot(_id).cardPtr;
    getSlot(_id).cardPtr = _card;
    return prevCard;
}

void Participant::visitSlots(std::function<void(ParticipantSlot&)> _visitor)
{
    unsigned short slotLeft = m_currentNumberOfSlots;
    // Visit only active slots
    for (auto& slot : m_slots)
    {
        if (slot.wasRemoved)
            continue;
        
        _visitor(slot);

        --slotLeft;
        if (slotLeft == 0)
            break;
    }
}

const ParticipantSlot& Participant::getSlot(ParticipantSlotId _id) const
{
    CN_ASSERT(m_slots.size() > _id);
    return m_slots.at(_id);
}

ParticipantSlot& Participant::getSlot(ParticipantSlotId _id)
{
    CN_ASSERT(m_slots.size() > _id);
    return m_slots.at(_id);
}

void Participant::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_nameText);
}

} // namespace cn::client::game