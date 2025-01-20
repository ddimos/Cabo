#pragma once
#include "core/event/Types.hpp"

namespace cn::events
{

namespace id
{

enum : core::event::EventId
{
    // Connecction events
    PeerConnected,
    PeerDisconnected,

    // Input events
    KeyReleased,
    MouseButtonReleased,
    MouseMoved,

    // Net events
    // PlayerJoined,
    // PlayerLeft,
    PlayerInfoUpdate,


    // Player action events
    LocalPlayerClickSlot,
    PlayerClickDeck
};

} // namespace id

} // namespace cn::events
