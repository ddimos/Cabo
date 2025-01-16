#include "net/Manager.hpp"

#include "core/event/Dispatcher.hpp"
#include "events/NetworkEvents.hpp"

#include "core/Assert.hpp"
#include "core/Log.hpp"

namespace
{

constexpr nsf::Port SERVER_PORT = 20475;
constexpr nsf::Port CLIENT_PORT = 20480;

}

namespace cn::net
{

Manager::Manager(core::Context& _contextRef)
    : m_contextRef(_contextRef)
{
}

Manager::~Manager() = default;

void Manager::init(bool _isServer)
{
    auto& eventDispatcherRef = m_contextRef.get<core::event::Dispatcher>();

    nsf::Config config;
    nsf::NSFCallbacks callbacks;

    config.port = (_isServer) ? SERVER_PORT : CLIENT_PORT;
    config.isServer = _isServer;

    callbacks.onConnected = [&eventDispatcherRef](nsf::PeerID _peerId){
        CN_LOG_FRM("Peer connected: {}", _peerId);
        eventDispatcherRef.send<events::PeerConnectedEvent>(_peerId);
    };
    callbacks.onDisconnected = [&eventDispatcherRef](nsf::PeerID _peerId){
        CN_LOG_FRM("Peer disconnected: {}", _peerId);
        eventDispatcherRef.send<events::PeerDisconnected>(_peerId);
    };
    callbacks.onReceived = [](nsf::NetworkMessage&& _message){
        CN_LOG_FRM("Received from: {}", _message.getPeerId());
    };

    m_network = nsf::createNSF(config, callbacks);
    CN_LOG("Network is created!");
    // todo show this on the ui
    CN_LOG_FRM("The public address : {}", m_network->getPublicAddress().toString());
    CN_LOG_FRM("The local address : {}", m_network->getLocalAddress().toString());

}

void Manager::connect(nsf::NetworkAddress _address)
{    
    CN_ASSERT(!m_network->isServer());
    m_network->connect(_address);
}

void Manager::updateReceive()
{
    m_network->updateReceive();
}

void Manager::updateSend()
{
    m_network->updateSend();
}

} // namespace cn::net
