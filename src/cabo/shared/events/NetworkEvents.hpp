#pragma once
#include "core/event/Event.hpp"

#include "shared/events/EventIds.hpp"

#include <nsf/Types.hpp>
#include <string>
#include <vector>

namespace cn::events
{

class BaseNetEvent : public core::event::Event
{
public:
    BaseNetEvent() = default;
    BaseNetEvent(nsf::PeerID _senderPeerId)
        : m_senderPeerId(_senderPeerId)
    {}

    nsf::PeerID m_senderPeerId = nsf::PEER_ID_INVALID;
};

class PlayerJoinAcceptEvent final: public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerJoinAccept)

    PlayerJoinAcceptEvent() = default;
    PlayerJoinAcceptEvent(nsf::PeerID _senderPeerId)
        : BaseNetEvent(_senderPeerId)
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
    PlayerInfoUpdateEvent(nsf::PeerID _senderPeerId)
        : BaseNetEvent(_senderPeerId)
    {}

    std::vector<Player> m_players;
};

class PlayerReadyEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::PlayerReady)

    PlayerReadyEvent() = default;
    PlayerReadyEvent(nsf::PeerID _senderPeerId)
        : BaseNetEvent(_senderPeerId)
    {}
};

class StartGameEvent final : public BaseNetEvent
{
public:
    CN_EVENT(id::StartGame)

    StartGameEvent() = default;
    StartGameEvent(nsf::PeerID _senderPeerId)
        : BaseNetEvent(_senderPeerId)
    {}
};

} // namespace cn::events
