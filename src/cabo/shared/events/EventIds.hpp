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
    // PlayerJoined,
    // PlayerLeft,
    BoardStateUpdate,
    DiscardCard,
    FinishGame,
    MatchCard,
    PlayerJoinAccept,
    PlayerReadyStatusUpdate,
    PlayerSlotUpdate,
    PlayerTurnUpdate,
    PlayerUpdate,
    ProvideCard,
    RemotePlayerInput,
    RemotePlayerInput2,
    ServerCommand,
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
