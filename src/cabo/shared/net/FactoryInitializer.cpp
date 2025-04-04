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

            if (event.m_inputType == shared::game::InputType::ClickPile)
            {
                _buffer << static_cast<uint8_t>(std::get<shared::game::PileType>(event.m_data));
            }
            else if (event.m_inputType == shared::game::InputType::ClickSlot)
            {
                auto dataStruct = std::get<shared::game::ClickSlotInputData>(event.m_data);
                _buffer << static_cast<uint8_t>(dataStruct.slotId);
                _buffer << static_cast<uint8_t>(dataStruct.playerId);
            }
            else if (event.m_inputType == shared::game::InputType::Action)
            {
                _buffer << static_cast<uint8_t>(std::get<shared::game::ActionType>(event.m_data));
            }
            else if (event.m_inputType == shared::game::InputType::SwapDecision)
            {
                _buffer << static_cast<uint8_t>(std::get<bool>(event.m_data));
            }
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::RemotePlayerInputNetEvent&>(_event);
            _buffer >> event.m_playerId;
            uint8_t inputType = 0;
            _buffer >> inputType;
            event.m_inputType = static_cast<shared::game::InputType>(inputType);
            
            [[maybe_unused]] uint8_t data = 0;
            if (event.m_inputType == shared::game::InputType::ClickPile)
            {
                _buffer >> data;
                event.m_data = static_cast<shared::game::PileType>(data);
            }
            else if (event.m_inputType == shared::game::InputType::ClickSlot)
            {
                shared::game::ClickSlotInputData dataStruct;
                _buffer >> data;
                dataStruct.slotId = data;
                _buffer >> data;
                dataStruct.playerId = data;
                event.m_data = dataStruct;
            }
            else if (event.m_inputType == shared::game::InputType::Action)
            {
                _buffer >> data;
                event.m_data = static_cast<shared::game::ActionType>(data);
            }
            else if (event.m_inputType == shared::game::InputType::SwapDecision)
            {
                _buffer >> data;
                event.m_data = static_cast<bool>(data);
            }
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
    _factoryRef.add(
        events::id::MatchCard,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::MatchCardNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_rank);
            _buffer << static_cast<uint8_t>(event.m_suit);
            _buffer << event.m_isMatched;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::MatchCardNetEvent&>(_event);
            uint8_t rank;
            uint8_t suit;
            _buffer >> rank;
            _buffer >> suit;
            event.m_rank = static_cast<shared::game::Card::Rank>(rank);
            event.m_suit = static_cast<shared::game::Card::Suit>(suit);
            _buffer >> event.m_isMatched;
        },
        [](){
            return std::make_unique<events::MatchCardNetEvent>();
        }
    );
    _factoryRef.add(
        events::id::PlayerSlotUpdate,
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::PlayerSlotUpdateNetEvent&>(_event);
            _buffer << event.m_playerId;
            _buffer << event.m_slotId;
            _buffer << event.m_wasAdded;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerSlotUpdateNetEvent&>(_event);
            _buffer >> event.m_playerId;
            _buffer >> event.m_slotId;
            _buffer >> event.m_wasAdded;
        },
        [](){
            return std::make_unique<events::PlayerSlotUpdateNetEvent>();
        }
    );
}

} // namespace cn::net
