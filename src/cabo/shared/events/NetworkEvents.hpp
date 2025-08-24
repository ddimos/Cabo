#pragma once
#include "core/event/Event.hpp"

#include "shared/events/EventIds.hpp"
#include "shared/game/Types.hpp"
#include "shared/player/Types.hpp"

#include "SFML/System/Time.hpp"

#include <nsf/Types.hpp>
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

    PlayerId m_playerId{}; 
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

class FinishGameNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::FinishGame)
    
    FinishGameNetEvent() = default;
};

class RemotePlayerInputNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::RemotePlayerInput)

    RemotePlayerInputNetEvent() = default;
    RemotePlayerInputNetEvent(PlayerId _playerId, shared::game::PlayerInputType _type)
        : m_playerId(_playerId), m_type(_type)
    {}
    RemotePlayerInputNetEvent(PlayerId _playerId, shared::game::PlayerInputType _type, shared::game::PlayerInputDataVariant _data)
        : m_playerId(_playerId), m_type(_type), m_data(_data)
    {}

    PlayerId m_playerId{};// TODO remove this, it is sent from a local player to server
    shared::game::PlayerInputType m_type = shared::game::PlayerInputType::Grab;
    shared::game::PlayerInputDataVariant m_data = std::monostate();
};

class ServerCommandNetEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::ServerCommand)

    ServerCommandNetEvent() = default;
    ServerCommandNetEvent(shared::game::ServerCommandType _type)
        : m_type(_type)
    {}
    ServerCommandNetEvent(shared::game::ServerCommandType _type, shared::game::ServerCommandDataVariant _data)
        : m_type(_type), m_data(_data)
    {}

    shared::game::ServerCommandType m_type = shared::game::ServerCommandType::PlayerInteractsWithCard;
    shared::game::ServerCommandDataVariant m_data = std::monostate();
};

} // namespace cn::events
