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

class PlayerStepUpdateEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerStepUpdate)

    PlayerStepUpdateEvent() = default;
    PlayerStepUpdateEvent(PlayerId _playerId, shared::game::StepId _stepId)
        : m_playerId(_playerId), m_stepId(_stepId)
    {}

    PlayerId m_playerId = PlayerIdInvalid;
    shared::game::StepId m_stepId = shared::game::StepId::Idle;
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

} // namespace cn::events
