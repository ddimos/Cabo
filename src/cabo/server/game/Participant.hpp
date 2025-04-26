#pragma once

#include "core/object/Object.hpp"

#include "shared/player/Types.hpp"

#include "server/game/Card.hpp"
#include "server/game/Types.hpp"

#include <functional>
#include <vector>

namespace cn::core
{
    class Context;
}

namespace cn::server::game
{

// TODO to think if it makes sense to united it with the client version?

class Participant final : public core::object::Object
{
public:
    Participant(const core::Context& _context, PlayerId _id,
        std::vector<ParticipantSlot>&& _slots,
        unsigned short _initialNumberOfSlots);

    PlayerId getId() const { return m_id; }

    ParticipantSlotId addSlot();
    void removeSlot(ParticipantSlotId _id);

    const ParticipantSlot& getSlot(ParticipantSlotId _id) const;
    Card* getCard(ParticipantSlotId _id) const;

    Card* replace(ParticipantSlotId _id, Card* _card);

    void visitSlots(std::function<void(ParticipantSlot&)> _visitor);

private:
    ParticipantSlot& getSlot(ParticipantSlotId _id);

    PlayerId m_id = PlayerIdInvalid;

    std::vector<ParticipantSlot> m_slots;
    unsigned short m_currentNumberOfSlots = 0;
    ParticipantSlotId m_nextSlotIdToAdd = shared::game::ParticipantSlotIdInvalid;
};

} // namespace cn::server::game
