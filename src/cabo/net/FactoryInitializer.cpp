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
            _buffer << static_cast<uint8_t>(event.m_players.size());
            for (const auto& player : event.m_players)
            {
                _buffer << player.id;
                _buffer << player.name;
            }
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerInfoUpdateEvent&>(_event);
            uint8_t size = 0;
            _buffer >> size;
            std::vector<Player> players;
            players.reserve(size);
            for (uint8_t i = 0; i < size; ++i)
            {
                Player player;
                _buffer >> player.id;
                _buffer >> player.name;
                players.emplace_back(std::move(player));
            }
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
