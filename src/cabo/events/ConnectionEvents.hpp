#pragma once
#include "core/event/Event.hpp"
#include "events/EventIds.hpp"

#include <nsf/Types.hpp>

namespace cn::events
{

class PeerConnectedEvent final : public core::event::Event
{
public:
    CN_EVENT(id::PeerConnected)

    PeerConnectedEvent(nsf::PeerID _peerId)
        : core::event::Event(), m_peerId(_peerId)
    {}

    nsf::PeerID m_peerId = nsf::PEER_ID_INVALID;
};

class PeerDisconnectedEvent final : public core::event::Event
{
public:
    CN_EVENT(id::PeerDisconnected)

    PeerDisconnectedEvent(nsf::PeerID _peerId)
        : core::event::Event(), m_peerId(_peerId)
    {}

    nsf::PeerID m_peerId = nsf::PEER_ID_INVALID;
};

} // namespace cn::events
