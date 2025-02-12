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

} // namespace cn::events
