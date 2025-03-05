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
        [](){
            return std::make_unique<events::PlayerJoinAcceptNetEvent>();
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
        [](){
            return std::make_unique<events::PlayerInfoUpdateNetEvent>();
        }
    );
    _factoryRef.add(
        events::id::PlayerReady,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
        },
        [](){
            return std::make_unique<events::PlayerReadyNetEvent>();
        }
    );
    _factoryRef.add(
        events::id::StartGame,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
        },
        [](){
            return std::make_unique<events::StartGameNetEvent>();
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
        [](){
            return std::make_unique<events::BoardStateUpdateNetEvent>();
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
        [](){
            return std::make_unique<events::PlayerTurnUpdateNetEvent>();
        }
    );
    _factoryRef.add(
        events::id::RemotePlayerInput,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::RemotePlayerInputNetEvent&>(_event);
            _buffer << event.m_playerId;
            _buffer << static_cast<uint8_t>(event.m_inputType);

            uint8_t data = 0;
            if (event.m_inputType == shared::game::InputType::ClickPile)
                data = static_cast<uint8_t>(std::get<shared::game::PileType>(event.m_data));
            else if (event.m_inputType == shared::game::InputType::ClickSlot)
                data = static_cast<uint8_t>(std::get<shared::game::ParticipantSlotId>(event.m_data));
            else if (event.m_inputType == shared::game::InputType::DecideButton)
                data = static_cast<uint8_t>(std::get<shared::game::DecideButton>(event.m_data));
            
            if (event.m_inputType != shared::game::InputType::Cabo)
                _buffer << data;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::RemotePlayerInputNetEvent&>(_event);
            _buffer >> event.m_playerId;
            uint8_t inputType = 0;
            _buffer >> inputType;
            event.m_inputType = static_cast<shared::game::InputType>(inputType);
            
            if (event.m_inputType == shared::game::InputType::Cabo)
                return;
            
            uint8_t data = 0;
            _buffer >> data;
            
            if (event.m_inputType == shared::game::InputType::ClickPile)
                event.m_data = static_cast<shared::game::PileType>(data);
            else if (event.m_inputType == shared::game::InputType::ClickSlot)
                event.m_data = static_cast<shared::game::ParticipantSlotId>(data);
            else if (event.m_inputType == shared::game::InputType::DecideButton)
                event.m_data = static_cast<shared::game::DecideButton>(data);
        },
        [](){
            return std::make_unique<events::RemotePlayerInputNetEvent>();
        }
    );
    _factoryRef.add(
        events::id::ProvideCard,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::ProvideCardNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_rank);
            _buffer << static_cast<uint8_t>(event.m_suit);
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::ProvideCardNetEvent&>(_event);
            uint8_t rank;
            uint8_t suit;
            _buffer >> rank;
            _buffer >> suit;
            event.m_rank = static_cast<shared::game::Card::Rank>(rank);
            event.m_suit = static_cast<shared::game::Card::Suit>(suit);
        },
        [](){
            return std::make_unique<events::ProvideCardNetEvent>();
        }
    );
    _factoryRef.add(
        events::id::DiscardCard,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::DiscardCardNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_rank);
            _buffer << static_cast<uint8_t>(event.m_suit);
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::DiscardCardNetEvent&>(_event);
            uint8_t rank;
            uint8_t suit;
            _buffer >> rank;
            _buffer >> suit;
            event.m_rank = static_cast<shared::game::Card::Rank>(rank);
            event.m_suit = static_cast<shared::game::Card::Suit>(suit);
        },
        [](){
            return std::make_unique<events::DiscardCardNetEvent>();
        }
    );
}

} // namespace cn::net
