#pragma once
#include "core/event/Types.hpp"

namespace cn::events
{

namespace id
{

enum : core::event::EventId::Type
{
    // Connecction events
    PeerConnected,
    PeerDisconnected,

    // Input events
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    TextEntered,

    // Net events
    FinishGame,
    PlayerJoinAccept,
    PlayerReadyStatusUpdate,
    PlayerUpdate,
    RemotePlayerInput,
    ServerCommand,
    StartGame,

    // Player related
    PlayerPresenceChanged,
};

} // namespace id

} // namespace cn::events
