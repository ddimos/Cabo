#pragma once
#include "core/event/Event.hpp"
#include "events/EventIds.hpp"

#include "game/Types.hpp"

#include <SFML/Window/Event.hpp>

namespace cn::events
{

class PlayerClickDeckEvent final : public core::event::Event
{
public:
    CN_EVENT(Id::PlayerClickDeck)

    PlayerClickDeckEvent()
        : core::event::Event() 
    {}
};

class LocalPlayerClickSlotEvent final : public core::event::Event
{
public:
    CN_EVENT(Id::LocalPlayerClickSlot)

    LocalPlayerClickSlotEvent(game::PlayerSlotId _slotId, game::PlayerId _playerId)
        : core::event::Event(), slotId(_slotId), playerId(_playerId)
    {}

    game::PlayerSlotId slotId = game::PlayerSlotIdInvalid;
    game::PlayerId playerId = game::PlayerIdInvalid;
};

} // namespace cn::events
