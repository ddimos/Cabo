#include "server/game/Participant.hpp"

#include "core/math/Math.hpp"
#include "core/Context.hpp"

namespace cn::server::game
{

Participant::Participant(const core::Context& _context, PlayerId _id, std::vector<ParticipantSlot>&& _slots, unsigned short _initialNumberOfSlots)
    : m_id(_id)
    , m_slots(std::move(_slots))
    , m_currentNumberOfSlots(_initialNumberOfSlots)
    , m_nextSlotIdToAdd(_initialNumberOfSlots)
{
}

ParticipantSlotId Participant::addSlot()
{
    if (m_nextSlotIdToAdd >= m_slots.size())
    {
        return shared::game::ParticipantSlotIdInvalid;
    }
    CN_ASSERT(m_currentNumberOfSlots <= m_slots.size());
    
    m_currentNumberOfSlots++;
    m_nextSlotIdToAdd++;

    return m_nextSlotIdToAdd-1;
}

void Participant::removeSlot(ParticipantSlotId _id)
{
    if (m_currentNumberOfSlots == 0)
    {
        CN_ASSERT(false);
        return;
    }
    CN_ASSERT(!getSlot(_id).wasRemoved);

    getSlot(_id).wasRemoved = true;
    m_currentNumberOfSlots--;
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

Card* Participant::getCard(ParticipantSlotId _id) const
{
    return getSlot(_id).card;
}

Card* Participant::replace(ParticipantSlotId _id, Card* _card)
{
    auto prevCard = getSlot(_id).card;
    getSlot(_id).card = _card;
    return prevCard;
}

} // namespace cn::server::game
