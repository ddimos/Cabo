#pragma once
#include "core/event/Types.hpp"

namespace cn::events
{

namespace id
{

enum : core::event::EventId
{
    // Input events
    KeyReleased,
    MouseButtonReleased,
    MouseMoved,

    // Net events
    // PlayerJoined,
    // PlayerLeft,
    PeerConnected,
    PeerDisconnected,

    // Player action events
    LocalPlayerClickSlot,
    PlayerClickDeck
};

} // namespace id

} // namespace cn::events
