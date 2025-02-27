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
    DrawCard,
    BoardStateUpdate,
    PlayerInfoUpdate,
    PlayerJoinAccept,
    PlayerReady,
    PlayerTurnUpdate,
    RemotePlayerClickPile,
    RemotePlayerClickSlot,
    SeeCardInSlot,
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
