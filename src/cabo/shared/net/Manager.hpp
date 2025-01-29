#pragma once

#include "shared/net/Factory.hpp"

#include "core/Context.hpp"
#include <nsf/NSF.hpp>

#include <vector>

namespace cn::net
{

class Manager final
{
public:
    struct NetPlayer
    {
        nsf::PeerID peerId = nsf::PEER_ID_INVALID;
    };
    using Players = std::vector<NetPlayer>;
    
    Manager(core::Context& _contextRef, bool _isServer);
    ~Manager();

    void init();

    void connect(nsf::NetworkAddress _address);

    void send(bool _isReliable, const core::event::Event& _event);

    void updateReceive();
    void updateSend();

    // TODO check if I need this
    const Players& getPlayers() const { return m_connectedPeers; }

private:
    core::Context& m_contextRef;
    bool m_isServer = false;
    std::unique_ptr<nsf::INSF> m_network;

    Factory m_factory;

    Players m_connectedPeers;
};

} // namespace cn::net
