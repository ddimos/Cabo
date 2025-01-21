#include "net/Manager.hpp"

#include "core/event/Dispatcher.hpp"
#include "events/ConnectionEvents.hpp"
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

Manager::Manager(core::Context& _contextRef, bool _isServer)
    : m_contextRef(_contextRef), m_isServer(_isServer)
{
}

Manager::~Manager() = default;

void Manager::init()
{
    auto& eventDispatcherRef = m_contextRef.get<core::event::Dispatcher>();

    nsf::Config config;
    nsf::NSFCallbacks callbacks;

    config.port = (m_isServer) ? SERVER_PORT : CLIENT_PORT;
    config.isServer = m_isServer;

    callbacks.onConnected = [&eventDispatcherRef](nsf::PeerID _peerId){
        CN_LOG_FRM("Peer connected: {}", _peerId);
        eventDispatcherRef.send<events::PeerConnectedEvent>(_peerId);
    };
    callbacks.onDisconnected = [&eventDispatcherRef](nsf::PeerID _peerId){
        CN_LOG_FRM("Peer disconnected: {}", _peerId);
        eventDispatcherRef.send<events::PeerDisconnected>(_peerId);
    };
    callbacks.onReceived = [this, &eventDispatcherRef](nsf::NetworkMessage&& _message){
        CN_LOG_FRM("Received from: {}", _message.getPeerId());
        core::event::EventId eventId = core::event::EventIdInvalid;
        _message.m_data >> eventId;
        auto& slot = m_factory.get(eventId);
        auto eventPtr = slot.create(_message.getPeerId());
        slot.deserialize(*eventPtr, _message.m_data);

        eventDispatcherRef.sendDelayed(std::move(eventPtr));
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

void Manager::send(bool _isReliable, const core::event::Event& _event)
{
    nsf::NetworkMessage message;
    message.m_info = nsf::MessageInfo(_isReliable);
    core::event::EventId eventId = _event.getId();
    auto& slot = m_factory.get(eventId);
    message.m_data << eventId;
    slot.serialize(_event, message.m_data);

    m_network->send(std::move(message));
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
