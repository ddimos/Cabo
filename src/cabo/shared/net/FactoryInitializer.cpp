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
            auto& event = static_cast<const events::PlayerJoinAcceptNetEvent&>(_event);
            _buffer << event.m_playerId;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerJoinAcceptNetEvent&>(_event);
            _buffer >> event.m_playerId;
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::PlayerJoinAcceptNetEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event);
        }
    );
    _factoryRef.add(
        events::id::PlayerInfoUpdate,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::PlayerInfoUpdateNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_players.size());
            for (const auto& player : event.m_players)
            {
                _buffer << player.id;
                _buffer << player.name;
            }
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerInfoUpdateNetEvent&>(_event);
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
            auto event = std::make_unique<events::PlayerInfoUpdateNetEvent>();
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
            auto event = std::make_unique<events::PlayerReadyNetEvent>();
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
            auto event = std::make_unique<events::StartGameNetEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::BoardStateUpdate,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::BoardStateUpdateNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_boardState); //  TODO underlying?
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::BoardStateUpdateNetEvent&>(_event);
            uint8_t boardState = 0;
            _buffer >> boardState;
            event.m_boardState = static_cast<shared::game::BoardState>(boardState);
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::BoardStateUpdateNetEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::PlayerTurnUpdate,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::PlayerTurnUpdateNetEvent&>(_event);
            _buffer << event.m_playerId;
            _buffer << event.m_hasTurnStarted;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerTurnUpdateNetEvent&>(_event);
            _buffer >> event.m_playerId;
            _buffer >> event.m_hasTurnStarted;
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::PlayerTurnUpdateNetEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::RemotePlayerClickSlot,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            const auto& event = static_cast<const events::RemotePlayerClickSlotNetEvent&>(_event);
            _buffer << event.m_slotOwnerId;
            _buffer << event.m_slotId;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::RemotePlayerClickSlotNetEvent&>(_event);
            _buffer >> event.m_slotOwnerId;
            _buffer >> event.m_slotId;
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::RemotePlayerClickSlotNetEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::SeeCardInSlot,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::SeeCardInSlotNetEvent&>(_event);
            _buffer << event.m_slotOwnerId;
            _buffer << event.m_slotId;
            _buffer << static_cast<uint8_t>(event.m_rank);
            _buffer << static_cast<uint8_t>(event.m_suit);
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::SeeCardInSlotNetEvent&>(_event);
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
            auto event = std::make_unique<events::SeeCardInSlotNetEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::DrawCard,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::DrawCardNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_rank);
            _buffer << static_cast<uint8_t>(event.m_suit);
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::DrawCardNetEvent&>(_event);
            uint8_t rank;
            uint8_t suit;
            _buffer >> rank;
            _buffer >> suit;
            event.m_rank = static_cast<shared::game::Card::Rank>(rank);
            event.m_suit = static_cast<shared::game::Card::Suit>(suit);
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::DrawCardNetEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::RemotePlayerClickPile,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::RemotePlayerClickPileNetEvent&>(_event);
            _buffer << event.m_playerClickedOnDeck;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::RemotePlayerClickPileNetEvent&>(_event);
            _buffer >> event.m_playerClickedOnDeck;
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::RemotePlayerClickPileNetEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
}

} // namespace cn::net
