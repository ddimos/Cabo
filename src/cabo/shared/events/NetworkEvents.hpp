#pragma once
#include "core/event/Event.hpp"

#include "shared/events/EventIds.hpp"
#include "shared/game/Card.hpp"
#include "shared/game/StepId.hpp"
#include "shared/game/Types.hpp"
#include "shared/player/Types.hpp"

#include "SFML/System/Time.hpp"

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
    sf::Time m_receivedTime{};
    sf::Time m_sentTimeRttBased{};
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

class PlayerUpdateNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerUpdate)

    PlayerUpdateNetEvent() = default;
    PlayerUpdateNetEvent(const std::vector<Player>& _players)
        : m_players(_players)
    {}

    std::vector<Player> m_players;
};

class PlayerReadyStatusUpdateNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerReadyStatusUpdate)

    PlayerReadyStatusUpdateNetEvent() = default;
    PlayerReadyStatusUpdateNetEvent(const std::unordered_map<PlayerId, bool>& _players)
        : m_players(_players)
    {}

    std::unordered_map<PlayerId, bool> m_players;
};

class StartGameNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::StartGame)
    
    StartGameNetEvent() = default;
    StartGameNetEvent(uint32_t _seed)
        : m_seed(_seed)
    {}

    uint32_t m_seed = 0;
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

class RemotePlayerInputNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::RemotePlayerInput)

    RemotePlayerInputNetEvent() = default;
    RemotePlayerInputNetEvent(PlayerId _playerId, shared::game::InputType _inputType)
        : m_playerId(_playerId), m_inputType(_inputType)
    {}
    RemotePlayerInputNetEvent(PlayerId _playerId, shared::game::InputType _inputType, shared::game::InputDataVariant _data)
        : m_playerId(_playerId), m_inputType(_inputType), m_data(_data)
    {}

    PlayerId m_playerId = PlayerIdInvalid;
    shared::game::InputType m_inputType = shared::game::InputType::Cabo;
    shared::game::InputDataVariant m_data = std::monostate();
};

class ProvideCardNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::ProvideCard)

    ProvideCardNetEvent() = default;
    ProvideCardNetEvent(shared::game::Card::Rank _rank, shared::game::Card::Suit _suit, shared::game::CardId _cardId)
        : m_rank(_rank), m_suit(_suit), m_cardId(_cardId)
    {}

    shared::game::Card::Rank m_rank = shared::game::Card::Rank::Ace;
    shared::game::Card::Suit m_suit = shared::game::Card::Suit::Clubs;
    shared::game::CardId m_cardId{};
};

class DiscardCardNetEvent final : public BaseNetEvent   // It's not currently used 
{
public:
    CN_EVENT(id::DiscardCard)

    DiscardCardNetEvent() = default;
    DiscardCardNetEvent(shared::game::Card::Rank _rank, shared::game::Card::Suit _suit)
        : m_rank(_rank), m_suit(_suit)
    {}

    shared::game::Card::Rank m_rank = shared::game::Card::Rank::Ace;
    shared::game::Card::Suit m_suit = shared::game::Card::Suit::Clubs;
};

class MatchCardNetEvent final : public BaseNetEvent     // It's not currently used 
{
public:
    CN_EVENT(id::MatchCard)

    MatchCardNetEvent() = default;
    MatchCardNetEvent(shared::game::Card::Rank _rank, shared::game::Card::Suit _suit, bool _isMatched)
        : m_rank(_rank), m_suit(_suit), m_isMatched(_isMatched)
    {}

    shared::game::Card::Rank m_rank = shared::game::Card::Rank::Ace;
    shared::game::Card::Suit m_suit = shared::game::Card::Suit::Clubs;
    bool m_isMatched = false;
};

class PlayerSlotUpdateNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerSlotUpdate)

    PlayerSlotUpdateNetEvent() = default;
    PlayerSlotUpdateNetEvent(PlayerId _playerId, shared::game::ParticipantSlotId _slotId, bool _wasAdded)
        : m_playerId(_playerId), m_slotId(_slotId), m_wasAdded(_wasAdded)
    {}

    PlayerId m_playerId = PlayerIdInvalid;
    shared::game::ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;
    bool m_wasAdded = false;
};

} // namespace cn::events
