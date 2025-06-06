#pragma once
#include "core/event/Event.hpp"

#include "shared/events/EventIds.hpp"
#include "shared/game/Types.hpp"
#include "shared/player/Types.hpp"

#include <SFML/Window/Event.hpp>

namespace cn::events
{

class PlayerPresenceChangedEvent final : public core::event::Event
{
public:
    CN_EVENT(id::PlayerPresenceChanged)

    PlayerPresenceChangedEvent(PlayerId _id, bool _joined)
        : m_id(_id), m_joined(_joined)
    {}

    PlayerId m_id{};
    bool m_joined = false;
};

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

    shared::game::ParticipantSlotId slotId{};
    PlayerId slotOwnerId{};
};

class LocalPlayerClickDecideButtonEvent final : public core::event::Event
{
public:
    CN_EVENT(id::LocalPlayerClickDecideButton)

    LocalPlayerClickDecideButtonEvent(shared::game::ActionType _button)
        : m_button(_button)
    {}

    shared::game::ActionType m_button = shared::game::ActionType::Ability;
};

class LocalPlayerClickDecideSwapButtonEvent final : public core::event::Event
{
public:
    CN_EVENT(id::LocalPlayerClickDecideSwapButton)

    LocalPlayerClickDecideSwapButtonEvent(bool _swap)
        : m_swap(_swap)
    {}

    bool m_swap = false;
};

class LocalPlayerClickFinishButtonEvent final : public core::event::Event
{
public:
    CN_EVENT(id::LocalPlayerClickFinishButton)

    LocalPlayerClickFinishButtonEvent()
    {}
};

class LocalPlayerClickCaboButtonEvent final : public core::event::Event
{
public:
    CN_EVENT(id::LocalPlayerClickCaboButton)

    LocalPlayerClickCaboButtonEvent()
    {}
};

} // namespace cn::events
