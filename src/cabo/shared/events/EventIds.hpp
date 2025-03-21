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
    MatchCard,
    PlayerInfoUpdate,
    PlayerJoinAccept,
    PlayerReady,
    PlayerSlotUpdate,
    PlayerTurnUpdate,
    ProvideCard,
    RemotePlayerInput,
    ShowCardInSlot,
    StartGame,


    // Player action events
    LocalPlayerClickDecideButton,
    LocalPlayerClickDecideSwapButton,
    LocalPlayerClickDeck,
    LocalPlayerClickDiscard,
    LocalPlayerClickFinishButton,
    LocalPlayerClickSlot,
};

} // namespace id

} // namespace cn::events
