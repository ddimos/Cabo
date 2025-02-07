#include "server/game/Participant.hpp"

#include "core/math/Math.hpp"
#include "core/Context.hpp"

namespace cn::server::game
{

Participant::Participant(const core::Context& _context, PlayerId _id, bool _isLocal, std::map<ParticipantSlotId, ParticipantSlot>&& _slots, unsigned short _initialNumberOfSlots)
    : m_id(_id)
    , m_isLocal(_isLocal)
    , m_slots(std::move(_slots))
    , m_currentNumberOfSlots(_initialNumberOfSlots)
{
}

void Participant::addSlot()
{
    if (m_currentNumberOfSlots >= m_slots.size())
    {
        CN_ASSERT(false);
        return; // TODO ParticipantSlotIdInvalid
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
        assigned = true;
        break;
    }
    CN_ASSERT(assigned);
}

} // namespace cn::server::game
