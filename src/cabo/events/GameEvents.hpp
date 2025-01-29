#pragma once
#include "core/event/Event.hpp"

#include "shared/events/EventIds.hpp"
#include "shared/game/Types.hpp"
#include "shared/player/Types.hpp"

#include <SFML/Window/Event.hpp>

namespace cn::events
{

class PlayerClickDeckEvent final : public core::event::Event
{
public:
    CN_EVENT(id::PlayerClickDeck)

    PlayerClickDeckEvent()
        : core::event::Event() 
    {}
};

class LocalPlayerClickSlotEvent final : public core::event::Event
{
public:
    CN_EVENT(id::LocalPlayerClickSlot)

    LocalPlayerClickSlotEvent(shared::game::PlayerSlotId _slotId, PlayerId _playerId)
        : core::event::Event(), slotId(_slotId), playerId(_playerId)
    {}

    shared::game::PlayerSlotId slotId = shared::game::PlayerSlotIdInvalid;
    PlayerId playerId = PlayerIdInvalid;
};

} // namespace cn::events
