#include "shared/net/FactoryInitializer.hpp"
#include "shared/net/Factory.hpp"

#include "shared/events/NetworkEvents.hpp"

namespace cn::net
{

FactoryInitializer::FactoryInitializer(Factory& _factoryRef)
{
    _factoryRef.add(
        events::id::PlayerJoinAccept,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::PlayerJoinAcceptEvent&>(_event);
            _buffer << event.m_playerId;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerJoinAcceptEvent&>(_event);
            _buffer >> event.m_playerId;
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::PlayerJoinAcceptEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event);
        }
    );
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
            event.m_players.reserve(size);
            for (uint8_t i = 0; i < size; ++i)
            {
                Player player;
                _buffer >> player.id;
                _buffer >> player.name;
                event.m_players.emplace_back(std::move(player));
            }
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::PlayerInfoUpdateEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::PlayerReady,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::PlayerReadyEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::StartGame,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::StartGameEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::PlayerStepUpdate,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::PlayerStepUpdateEvent&>(_event);
            _buffer << event.m_playerId;
            _buffer << static_cast<uint8_t>(event.m_stepId); //  TODO underlying?
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerStepUpdateEvent&>(_event);
            _buffer >> event.m_playerId;
            uint8_t stepId = 0;
            _buffer >> stepId;
            event.m_stepId = static_cast<shared::game::StepId>(stepId);
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::PlayerStepUpdateEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::RemotePlayerClickSlot,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            const auto& event = static_cast<const events::RemotePlayerClickSlotEvent&>(_event);
            _buffer << event.m_slotOwnerId;
            _buffer << event.m_slotId;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::RemotePlayerClickSlotEvent&>(_event);
            _buffer >> event.m_slotOwnerId;
            _buffer >> event.m_slotId;
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::RemotePlayerClickSlotEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::SeeCardInSlot,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::SeeCardInSlotEvent&>(_event);
            _buffer << event.m_slotOwnerId;
            _buffer << event.m_slotId;
            _buffer << static_cast<uint8_t>(event.m_rank);
            _buffer << static_cast<uint8_t>(event.m_suit);
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::SeeCardInSlotEvent&>(_event);
            _buffer >> event.m_slotOwnerId;
            _buffer >> event.m_slotId;
            uint8_t rank;
            uint8_t suit;
            _buffer >> rank;
            _buffer >> suit;
            event.m_rank = static_cast<shared::game::Card::Rank>(rank);
            event.m_suit = static_cast<shared::game::Card::Suit>(suit);
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::SeeCardInSlotEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
}

} // namespace cn::net
