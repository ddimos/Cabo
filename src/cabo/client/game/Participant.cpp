#include "client/game/Participant.hpp"
#include "client/game/Card.hpp"
#include "client/game/SpriteSheet.hpp"

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

    for (unsigned i = 0; i < m_currentNumberOfSlots; ++i)
        m_slots[i].button->requestActivated();
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
        slot.cardImage->setPosition(client::menu::Position{ .m_position = (m_spawnPoint.pos + localPos - sf::Vector2f{70, 100}) });
        // slot.cardImage->setRotation(m_spawnPoint.angleDeg + 90.f);
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
        return; // ParticipantSlotIdInvalid
    }

    m_currentNumberOfSlots++;
    auto& slot = getSlot(_id);
    slot.button->requestActivated();
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
    if (slot.button->isActivated())
        slot.button->requestDeactivated();
    if (slot.cardImage->isActivated())
        slot.cardImage->requestDeactivated();
    
    // Or should I just erase it? 
    slot.wasRemoved = true;
}

void Participant::onStartShowingCardInSlot(ParticipantSlotId _id)
{
    const auto& slot = getSlot(_id);
    slot.button->requestDeactivated();
    // TODO if card is valid - show card

    // TODO start an animation
}

void Participant::onCardRecievedInSlot(ParticipantSlotId _id, Card _card)
{
    auto& slot = getSlot(_id);
    if (slot.wasRemoved)
        return;

    slot.card = _card;
    slot.cardImage->setTextureRect(game::spriteSheet::getCardTextureRect(_card.getRank(), _card.getSuit()));
    slot.isCardValid = true;

    slot.cardImage->requestActivated();
}

void Participant::onFinishShowingCardInSlot(ParticipantSlotId _id)
{
    auto slot = getSlot(_id);
    if (slot.wasRemoved)
        return;
    slot.cardImage->requestDeactivated();
    slot.button->requestActivated();
}

void Participant::onProvideCardInSlot(ParticipantSlotId _id, Card _card)
{
    auto& slot = getSlot(_id);

    slot.card = _card;
    slot.cardImage->setTextureRect(game::spriteSheet::getCardTextureRect(_card.getRank(), _card.getSuit()));
    slot.isCardValid = true;
}

const ParticipantSlot& Participant::getSlot(ParticipantSlotId _id) const
{
    CN_ASSERT(m_slots.contains(_id));
    return m_slots.at(_id);
}

ParticipantSlot& Participant::getSlot(ParticipantSlotId _id)
{
    CN_ASSERT(m_slots.contains(_id));
    return m_slots.at(_id);
}

void Participant::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_nameText);
}

} // namespace cn::client::game