#pragma once
#include "core/event/Event.hpp"

#include "shared/events/EventIds.hpp"
#include "shared/player/Types.hpp"

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

} // namespace cn::events
