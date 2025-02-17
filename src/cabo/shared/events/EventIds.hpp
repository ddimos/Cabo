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
    PlayerJoinAccept,
    PlayerReady,
    PlayerStepUpdate,
    RemotePlayerClickSlot,
    SeeCardInSlot,
    StartGame,


    // Player action events
    LocalPlayerClickSlot,
    LocalPlayerClickDeck,
    LocalPlayerClickDiscard
};

} // namespace id

} // namespace cn::events
