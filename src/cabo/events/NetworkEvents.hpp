#pragma once
#include "core/event/Event.hpp"

#include "shared/events/EventIds.hpp"
#include "shared/game/Card.hpp"
#include "shared/player/Types.hpp"

#include <nsf/Types.hpp>
#include <string>
#include <vector>

namespace cn::events
{

class PlayerInfoUpdateEvent final : public core::event::Event
{
public:
    CN_EVENT(id::PlayerInfoUpdate)

    PlayerInfoUpdateEvent() = default;
    PlayerInfoUpdateEvent(std::vector<Player>&& _players)
        : m_players(std::move(_players))
    {}
    PlayerInfoUpdateEvent(nsf::PeerID _peerId)
        : m_peerId(_peerId)
    {}

    nsf::PeerID m_peerId = nsf::PEER_ID_INVALID; // TODO senderPeerId
    std::vector<Player> m_players;
};

class PlayerReadyEvent final : public core::event::Event
{
public:
    CN_EVENT(id::PlayerReady)

    PlayerReadyEvent() = default;
    PlayerReadyEvent(nsf::PeerID _peerId)
        : m_peerId(_peerId)
    {}

    nsf::PeerID m_peerId = nsf::PEER_ID_INVALID;
};

class StartGameEvent final : public core::event::Event
{
public:
    CN_EVENT(id::StartGame)

    StartGameEvent() = default;
    StartGameEvent(nsf::PeerID _peerId)
        : m_peerId(_peerId)
    {}

    nsf::PeerID m_peerId = nsf::PEER_ID_INVALID;
};

} // namespace cn::events
