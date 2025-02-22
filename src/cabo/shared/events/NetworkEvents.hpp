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

class PlayerJoinAcceptEvent final: public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerJoinAccept)

    PlayerJoinAcceptEvent() = default;
    PlayerJoinAcceptEvent(PlayerId _playerId)
        : m_playerId(_playerId)
    {}

    PlayerId m_playerId = PlayerIdInvalid; 
};

class PlayerInfoUpdateEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerInfoUpdate)

    PlayerInfoUpdateEvent() = default;
    PlayerInfoUpdateEvent(const std::vector<Player>& _players)
        : m_players(_players)
    {}

    std::vector<Player> m_players;
};

class PlayerReadyEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerReady)

    PlayerReadyEvent() = default;
};

class StartGameEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::StartGame)
    
    StartGameEvent() = default;
};

class BoardStateUpdateEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::BoardStateUpdate)

    BoardStateUpdateEvent() = default;
    BoardStateUpdateEvent(shared::game::BoardState _boardState)
        : m_boardState(_boardState)
    {}

    shared::game::BoardState m_boardState = shared::game::BoardState::Start;
};

class PlayerTurnUpdateEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerTurnUpdate)

    PlayerTurnUpdateEvent() = default;
    PlayerTurnUpdateEvent(PlayerId _playerId, bool _hasTurnStarted)
        : m_playerId(_playerId), m_hasTurnStarted(_hasTurnStarted)
    {}

    PlayerId m_playerId = PlayerIdInvalid;
    bool m_hasTurnStarted = true;
};

class RemotePlayerClickSlotEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::RemotePlayerClickSlot)

    RemotePlayerClickSlotEvent() = default;
    RemotePlayerClickSlotEvent(PlayerId _slotOwnerId, shared::game::ParticipantSlotId _slotId)
        : m_slotOwnerId(_slotOwnerId), m_slotId(_slotId)
    {}

    PlayerId m_slotOwnerId = PlayerIdInvalid;
    shared::game::ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;
};

class SeeCardInSlotEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::SeeCardInSlot)

    SeeCardInSlotEvent() = default;
    SeeCardInSlotEvent(PlayerId _slotOwnerId, shared::game::ParticipantSlotId _slotId, shared::game::Card::Rank _rank, shared::game::Card::Suit _suit)
        : m_slotOwnerId(_slotOwnerId), m_slotId(_slotId), m_rank(_rank), m_suit(_suit)
    {}

    PlayerId m_slotOwnerId = PlayerIdInvalid;
    shared::game::ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;
    shared::game::Card::Rank m_rank = shared::game::Card::Rank::Ace;
    shared::game::Card::Suit m_suit = shared::game::Card::Suit::Clubs;
};

class DrawCardEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::DrawCard)

    DrawCardEvent() = default;
    DrawCardEvent(shared::game::Card::Rank _rank, shared::game::Card::Suit _suit)
        : m_rank(_rank), m_suit(_suit)
    {}

    shared::game::Card::Rank m_rank = shared::game::Card::Rank::Ace;
    shared::game::Card::Suit m_suit = shared::game::Card::Suit::Clubs;
};

class RemotePlayerClickPileEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::RemotePlayerClickPile)

    RemotePlayerClickPileEvent() = default;
    RemotePlayerClickPileEvent(bool _playerClickedOnDeck)
        : m_playerClickedOnDeck(_playerClickedOnDeck)
    {}

    bool m_playerClickedOnDeck = false;
};

} // namespace cn::events
