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
    BoardStateUpdate,
    DiscardCard,
    PlayerInfoUpdate,
    PlayerJoinAccept,
    PlayerReady,
    PlayerTurnUpdate,
    ProvideCard,
    RemotePlayerInput,
    StartGame,


    // Player action events
    LocalPlayerClickDecideButton,
    LocalPlayerClickDeck,
    LocalPlayerClickDiscard,
    LocalPlayerClickFinishButton,
    LocalPlayerClickSlot,
};

} // namespace id

} // namespace cn::events
