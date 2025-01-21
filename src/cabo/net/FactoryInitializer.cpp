#include "net/FactoryInitializer.hpp"
#include "net/Factory.hpp"

#include "events/NetworkEvents.hpp"

namespace cn::net
{

FactoryInitializer::FactoryInitializer(Factory& _factoryRef)
{
    _factoryRef.add(
        events::id::PlayerInfoUpdate,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::PlayerInfoUpdateEvent&>(_event);
            _buffer << event.m_name;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerInfoUpdateEvent&>(_event);
            _buffer >> event.m_name;
        },
        [](nsf::PeerID _peerId){
            return std::make_unique<events::PlayerInfoUpdateEvent>(_peerId);
        }
    );
    _factoryRef.add(
        events::id::PlayerReady,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
        },
        [](nsf::PeerID _peerId){
            return std::make_unique<events::PlayerReadyEvent>(_peerId);
        }
    );
    _factoryRef.add(
        events::id::StartGame,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
        },
        [](nsf::PeerID _peerId){
            return std::make_unique<events::StartGameEvent>(_peerId);
        }
    );
}

} // namespace cn::net
