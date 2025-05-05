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
    MouseButtonReleased,
    MouseMoved,

    // Net events
    // PlayerJoined,
    // PlayerLeft,
    BoardStateUpdate,
    DiscardCard,
    MatchCard,
    PlayerJoinAccept,
    PlayerReadyStatusUpdate,
    PlayerSlotUpdate,
    PlayerTurnUpdate,
    PlayerUpdate,
    ProvideCard,
    RemotePlayerInput,
    ShowCardInSlot,
    StartGame,

    // Player related
    PlayerPresenceChanged,

    // Player action events
    LocalPlayerClickCaboButton,
    LocalPlayerClickDecideButton,
    LocalPlayerClickDecideSwapButton,
    LocalPlayerClickDeck,
    LocalPlayerClickDiscard,
    LocalPlayerClickFinishButton,
    LocalPlayerClickSlot,
};

} // namespace id

} // namespace cn::events
