#pragma once
#include "core/event/Event.hpp"

#include "shared/events/EventIds.hpp"
#include "shared/game/Card.hpp"
#include "shared/game/StepId.hpp"
#include "shared/game/Types.hpp"
#include "shared/player/Types.hpp"

#include <nsf/Types.hpp>
#include <string>
#include <vector>

namespace cn::events
{

class BaseNetEvent : public core::event::Event
{
public:
    BaseNetEvent() = default;
    
    nsf::PeerID m_senderPeerId = nsf::PEER_ID_INVALID;
};

class PlayerJoinAcceptNetEvent final: public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerJoinAccept)

    PlayerJoinAcceptNetEvent() = default;
    PlayerJoinAcceptNetEvent(PlayerId _playerId)
        : m_playerId(_playerId)
    {}

    PlayerId m_playerId = PlayerIdInvalid; 
};

class PlayerInfoUpdateNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerInfoUpdate)

    PlayerInfoUpdateNetEvent() = default;
    PlayerInfoUpdateNetEvent(const std::vector<Player>& _players)
        : m_players(_players)
    {}

    std::vector<Player> m_players;
};

class PlayerReadyNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerReady)

    PlayerReadyNetEvent() = default;
};

class StartGameNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::StartGame)
    
    StartGameNetEvent() = default;
};

class BoardStateUpdateNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::BoardStateUpdate)

    BoardStateUpdateNetEvent() = default;
    BoardStateUpdateNetEvent(shared::game::BoardState _boardState)
        : m_boardState(_boardState)
    {}

    shared::game::BoardState m_boardState = shared::game::BoardState::Start;
};

class PlayerTurnUpdateNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerTurnUpdate)

    PlayerTurnUpdateNetEvent() = default;
    PlayerTurnUpdateNetEvent(PlayerId _playerId, bool _hasTurnStarted)
        : m_playerId(_playerId), m_hasTurnStarted(_hasTurnStarted)
    {}

    PlayerId m_playerId = PlayerIdInvalid;
    bool m_hasTurnStarted = true;
};

class RemotePlayerClickSlotNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::RemotePlayerClickSlot)

    RemotePlayerClickSlotNetEvent() = default;
    RemotePlayerClickSlotNetEvent(PlayerId _slotOwnerId, shared::game::ParticipantSlotId _slotId)
        : m_slotOwnerId(_slotOwnerId), m_slotId(_slotId)
    {}

    PlayerId m_slotOwnerId = PlayerIdInvalid;
    shared::game::ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;
};

class SeeCardInSlotNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::SeeCardInSlot)

    SeeCardInSlotNetEvent() = default;
    SeeCardInSlotNetEvent(PlayerId _slotOwnerId, shared::game::ParticipantSlotId _slotId, shared::game::Card::Rank _rank, shared::game::Card::Suit _suit)
        : m_slotOwnerId(_slotOwnerId), m_slotId(_slotId), m_rank(_rank), m_suit(_suit)
    {}

    PlayerId m_slotOwnerId = PlayerIdInvalid;
    shared::game::ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;
    shared::game::Card::Rank m_rank = shared::game::Card::Rank::Ace;
    shared::game::Card::Suit m_suit = shared::game::Card::Suit::Clubs;
};

class DrawCardNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::DrawCard)

    DrawCardNetEvent() = default;
    DrawCardNetEvent(shared::game::Card::Rank _rank, shared::game::Card::Suit _suit)
        : m_rank(_rank), m_suit(_suit)
    {}

    shared::game::Card::Rank m_rank = shared::game::Card::Rank::Ace;
    shared::game::Card::Suit m_suit = shared::game::Card::Suit::Clubs;
};

class RemotePlayerClickPileNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::RemotePlayerClickPile)

    RemotePlayerClickPileNetEvent() = default;
    RemotePlayerClickPileNetEvent(bool _playerClickedOnDeck)
        : m_playerClickedOnDeck(_playerClickedOnDeck)
    {}

    bool m_playerClickedOnDeck = false;
};

} // namespace cn::events
