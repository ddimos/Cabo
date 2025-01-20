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
        [](){
            return std::make_unique<events::PlayerInfoUpdateEvent>();
        }
    );
}

} // namespace cn::net
