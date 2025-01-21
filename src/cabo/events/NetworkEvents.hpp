#pragma once
#include "core/event/Event.hpp"
#include "events/EventIds.hpp"

#include <nsf/Types.hpp>
#include <string>

namespace cn::events
{

class PlayerInfoUpdateEvent final : public core::event::Event
{
public:
    CN_EVENT(id::PlayerInfoUpdate)

    PlayerInfoUpdateEvent() = default;
    PlayerInfoUpdateEvent(const std::string& _name)
        : m_name(_name)
    {}
    PlayerInfoUpdateEvent(nsf::PeerID _peerId)
        : m_peerId(_peerId)
    {}

    nsf::PeerID m_peerId = nsf::PEER_ID_INVALID;
    std::string m_name;
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
