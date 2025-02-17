#pragma once
#include "core/event/Event.hpp"

#include "shared/events/EventIds.hpp"
#include "shared/game/Types.hpp"
#include "shared/player/Types.hpp"

#include <SFML/Window/Event.hpp>

namespace cn::events
{

class LocalPlayerClickDeckEvent final : public core::event::Event
{
public:
    CN_EVENT(id::LocalPlayerClickDeck)

    LocalPlayerClickDeckEvent()
        : core::event::Event() 
    {}
};

class LocalPlayerClickDiscardEvent final : public core::event::Event
{
public:
    CN_EVENT(id::LocalPlayerClickDiscard)

    LocalPlayerClickDiscardEvent()
        : core::event::Event() 
    {}
};

class LocalPlayerClickSlotEvent final : public core::event::Event
{
public:
    CN_EVENT(id::LocalPlayerClickSlot)

    LocalPlayerClickSlotEvent(shared::game::ParticipantSlotId _slotId, PlayerId _slotOwnerId)
        : core::event::Event(), slotId(_slotId), slotOwnerId(_slotOwnerId)
    {}

    shared::game::ParticipantSlotId slotId = shared::game::ParticipantSlotIdInvalid;
    PlayerId slotOwnerId = PlayerIdInvalid;
};

} // namespace cn::events
