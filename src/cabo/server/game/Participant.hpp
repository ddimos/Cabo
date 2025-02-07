#pragma once

#include "core/object/Object.hpp"

#include "shared/player/Types.hpp"

#include "server/game/Card.hpp"
#include "server/game/Types.hpp"

#include <functional>
#include <map>

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
    Participant(const core::Context& _context, PlayerId _id, bool _isLocal,
        std::map<ParticipantSlotId, ParticipantSlot>&& _slots,
        unsigned short _initialNumberOfSlots);

    PlayerId getId() const { return m_id; }
    bool isLocal() const { return m_isLocal; }

    void addSlot();
    void removeSlot(ParticipantSlotId _id);

    const ParticipantSlot& getSlot(ParticipantSlotId _id) const;
    Card* getCard(ParticipantSlotId _id) const;

    Card* replace(ParticipantSlotId _id, Card* _card);

    void deal(Card* _card);

private:
    PlayerId m_id = PlayerIdInvalid;
    bool m_isLocal = false;

    std::map<ParticipantSlotId, ParticipantSlot> m_slots;
    unsigned short m_currentNumberOfSlots = 0;
};

} // namespace cn::server::game
