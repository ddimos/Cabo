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
        events::id::BoardStateUpdate,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::BoardStateUpdateEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_boardState); //  TODO underlying?
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::BoardStateUpdateEvent&>(_event);
            uint8_t boardState = 0;
            _buffer >> boardState;
            event.m_boardState = static_cast<shared::game::BoardState>(boardState);
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::BoardStateUpdateEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::PlayerTurnUpdate,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::PlayerTurnUpdateEvent&>(_event);
            _buffer << event.m_playerId;
            _buffer << event.m_hasTurnStarted;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerTurnUpdateEvent&>(_event);
            _buffer >> event.m_playerId;
            _buffer >> event.m_hasTurnStarted;
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::PlayerTurnUpdateEvent>();
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
    _factoryRef.add(
        events::id::DrawCard,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::DrawCardEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_rank);
            _buffer << static_cast<uint8_t>(event.m_suit);
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::DrawCardEvent&>(_event);
            uint8_t rank;
            uint8_t suit;
            _buffer >> rank;
            _buffer >> suit;
            event.m_rank = static_cast<shared::game::Card::Rank>(rank);
            event.m_suit = static_cast<shared::game::Card::Suit>(suit);
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::DrawCardEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
    _factoryRef.add(
        events::id::RemotePlayerClickPile,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::RemotePlayerClickPileEvent&>(_event);
            _buffer << event.m_playerClickedOnDeck;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::RemotePlayerClickPileEvent&>(_event);
            _buffer >> event.m_playerClickedOnDeck;
        },
        [](nsf::PeerID _peerId){
            auto event = std::make_unique<events::RemotePlayerClickPileEvent>();
            event->m_senderPeerId = _peerId;
            return std::move(event); 
        }
    );
}

} // namespace cn::net
