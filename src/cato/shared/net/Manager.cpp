#include "shared/net/Manager.hpp"

#include "shared/events/ConnectionEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "core/event/Dispatcher.hpp"
#include "core/Assert.hpp"
#include "core/Log.hpp"

#include <algorithm>

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
    auto& systemClockRef = m_contextRef.get<sf::Clock>();

    nsf::Config config;
    nsf::NSFCallbacks callbacks;

    config.port = (m_isServer) ? SERVER_PORT : CLIENT_PORT;
    config.isServer = m_isServer;
    
    callbacks.onConnected = [this, &eventDispatcherRef](nsf::PeerID _peerId){
        CN_LOG_I_FRM("Peer connected: {}", _peerId);
        m_connectedPeers.push_back(NetPlayer{ .peerId = _peerId });
        eventDispatcherRef.send<events::PeerConnectedEvent>(_peerId);
    };
    callbacks.onDisconnected = [this, &eventDispatcherRef](nsf::PeerID _peerId){
        CN_LOG_I_FRM("Peer disconnected: {}", _peerId);
        eventDispatcherRef.send<events::PeerDisconnectedEvent>(_peerId);
        m_connectedPeers.erase( 
            std::remove_if(m_connectedPeers.begin(), m_connectedPeers.end(),
            [_peerId](NetPlayer _player){
                return _player.peerId == _peerId;
            }),
            m_connectedPeers.end()
        );
    };
    callbacks.onReceived = [this, &eventDispatcherRef, &systemClockRef](nsf::NetworkMessage&& _message){
        core::event::EventId eventId{};
        {
            core::event::EventId::Type id = 0;
            _message.m_data >> id;
            eventId = core::event::EventId(id);
        }
        auto& slot = m_factory.get(eventId);
        auto eventPtr = slot.create();
        {
            auto receivedTime = systemClockRef.getElapsedTime();
            float rtt = m_network->getRtt(_message.getPeerId());
            auto& netEvent = static_cast<events::BaseNetEvent&>(*eventPtr);
            netEvent.m_senderPeerId = _message.getPeerId();
            netEvent.m_receivedTime = receivedTime;
            netEvent.m_sentTimeRttBased = receivedTime - sf::Time(sf::seconds(rtt / 2.f));
        
            CN_LOG_D_FRM("Received from: {}, event: {}, time: {}, rtt: {}", _message.getPeerId(), eventId.value(), netEvent.m_receivedTime.asSeconds(), rtt);
        }
        
        slot.deserialize(*eventPtr, _message.m_data);
        eventDispatcherRef.sendDelayed(std::move(eventPtr));
    };

    m_network = nsf::createNSF(config, callbacks);
    CN_LOG_I("Network is created!");
    // todo show this on the ui
    CN_LOG_I_FRM("The public address : {}", m_network->getPublicAddress().toString());
    CN_LOG_I_FRM("The local address : {}", m_network->getLocalAddress().toString());
}

void Manager::connect(nsf::NetworkAddress _address)
{    
    CN_ASSERT(!m_network->isServer());
    m_network->connect(_address);
}

void Manager::send(const core::event::Event& _event)
{
    send(_event, nsf::MessageInfo::Type::BRODCAST, true, nsf::PEER_ID_INVALID);
}

void Manager::send(const core::event::Event& _event, bool _isReliable)
{
    send(_event, nsf::MessageInfo::Type::BRODCAST, _isReliable, nsf::PEER_ID_INVALID);
}

void Manager::send(const core::event::Event& _event, nsf::PeerID _unicastPeerId)
{
    send(_event, nsf::MessageInfo::Type::UNICAST, true, _unicastPeerId);
}

void Manager::send(const core::event::Event& _event, nsf::MessageInfo::Type _type, bool _isReliable, nsf::PeerID _peerId)
{
    CN_ASSERT(
        (_type == nsf::MessageInfo::Type::BRODCAST && _peerId == nsf::PEER_ID_INVALID) ||
        (_type == nsf::MessageInfo::Type::UNICAST && _peerId != nsf::PEER_ID_INVALID) ||
        (_type == nsf::MessageInfo::Type::EXCLUDE_BRODCAST && _peerId != nsf::PEER_ID_INVALID)
    );

    nsf::NetworkMessage message;
    message.m_info = nsf::MessageInfo(
        _type,
        _peerId,
        _isReliable
    );
    core::event::EventId eventId = _event.getId();
    auto& slot = m_factory.get(eventId);
    message.m_data << eventId.value();
    slot.serialize(_event, message.m_data);
    
    CN_LOG_D_FRM("Send {} to: {}, event: {}", 
        (_type == nsf::MessageInfo::Type::BRODCAST ? 'b' : (_type == nsf::MessageInfo::Type::UNICAST ? 'u' : 'e')),
        _peerId, eventId.value()
    );
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
