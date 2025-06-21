#include "shared/net/FactoryInitializer.hpp"
#include "shared/net/Factory.hpp"

#include "shared/events/NetworkEvents.hpp"

namespace
{

template<typename TId>
void deserializeId(TId& _id, nsf::Buffer& _buffer)
{
    typename TId::Type id = 0;
    _buffer >> id;
    _id = TId(id);
}

template<typename T, typename TEnum>
void deserializeEnum(TEnum& _value, nsf::Buffer& _buffer)
{
    T value = 0;
    _buffer >> value;
    _value = static_cast<TEnum>(value);
}

} // namespace

namespace cn::net
{

FactoryInitializer::FactoryInitializer(Factory& _factoryRef)
{
    _factoryRef.add(
        core::event::EventId(events::id::PlayerJoinAccept),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::PlayerJoinAcceptNetEvent&>(_event);
            _buffer << event.m_playerId.value();
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerJoinAcceptNetEvent&>(_event);
            deserializeId(event.m_playerId, _buffer);
        },
        [](){
            return std::make_unique<events::PlayerJoinAcceptNetEvent>();
        }
    );
    _factoryRef.add(
        core::event::EventId(events::id::PlayerUpdate),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::PlayerUpdateNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_players.size());
            for (const auto& player : event.m_players)
            {
                _buffer << player.id.value();
                _buffer << player.name;
            }
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerUpdateNetEvent&>(_event);
            uint8_t size = 0;
            _buffer >> size;
            event.m_players.reserve(size);
            for (uint8_t i = 0; i < size; ++i)
            {
                Player player;
                deserializeId(player.id, _buffer);
                _buffer >> player.name;
                event.m_players.emplace_back(std::move(player));
            }
        },
        [](){
            return std::make_unique<events::PlayerUpdateNetEvent>();
        }
    );
    _factoryRef.add(
        core::event::EventId(events::id::PlayerReadyStatusUpdate),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::PlayerReadyStatusUpdateNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_players.size());
            for (const auto& [id, ready] : event.m_players)
            {
                _buffer << id.value();
                _buffer << ready;
            }
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerReadyStatusUpdateNetEvent&>(_event);
            uint8_t size = 0;
            _buffer >> size;
            event.m_players.reserve(size);
            for (uint8_t i = 0; i < size; ++i)
            {
                PlayerId id;
                bool ready;
                deserializeId(id, _buffer);
                _buffer >> ready;
                event.m_players.emplace(id, ready);
            }
        },
        [](){
            return std::make_unique<events::PlayerReadyStatusUpdateNetEvent>();
        }
    );
    _factoryRef.add(
        core::event::EventId(events::id::StartGame),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::StartGameNetEvent&>(_event);
            _buffer << event.m_seed;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::StartGameNetEvent&>(_event);
            _buffer >> event.m_seed;
        },
        [](){
            return std::make_unique<events::StartGameNetEvent>();
        }
    );
    _factoryRef.add(
        core::event::EventId(events::id::FinishGame),
        [](const core::event::Event&, nsf::Buffer&){},
        [](core::event::Event&, nsf::Buffer&){},
        [](){
            return std::make_unique<events::FinishGameNetEvent>();
        }
    );
    _factoryRef.add(
        core::event::EventId(events::id::BoardStateUpdate),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::BoardStateUpdateNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_boardState); //  TODO underlying?
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::BoardStateUpdateNetEvent&>(_event);
            deserializeEnum<uint8_t>(event.m_boardState, _buffer);
        },
        [](){
            return std::make_unique<events::BoardStateUpdateNetEvent>();
        }
    );
    _factoryRef.add(
        core::event::EventId(events::id::PlayerTurnUpdate),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::PlayerTurnUpdateNetEvent&>(_event);
            _buffer << event.m_playerId.value();
            _buffer << event.m_hasTurnStarted;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerTurnUpdateNetEvent&>(_event);
            deserializeId(event.m_playerId, _buffer);
            _buffer >> event.m_hasTurnStarted;
        },
        [](){
            return std::make_unique<events::PlayerTurnUpdateNetEvent>();
        }
    );
    _factoryRef.add(
        core::event::EventId(events::id::RemotePlayerInput),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::RemotePlayerInputNetEvent&>(_event);
            _buffer << event.m_playerId.value();
            _buffer << static_cast<uint8_t>(event.m_inputType);

            if (event.m_inputType == shared::game::InputType::ClickPile)
            {
                _buffer << static_cast<uint8_t>(std::get<shared::game::PileType>(event.m_data));
            }
            else if (event.m_inputType == shared::game::InputType::ClickSlot)
            {
                auto dataStruct = std::get<shared::game::ClickSlotInputData>(event.m_data);
                _buffer << static_cast<uint8_t>(dataStruct.slotId.value());
                _buffer << static_cast<uint8_t>(dataStruct.playerId.value());
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
            deserializeId(event.m_playerId, _buffer);
            deserializeEnum<uint8_t>(event.m_inputType, _buffer);
            
            [[maybe_unused]] uint8_t data = 0;
            if (event.m_inputType == shared::game::InputType::ClickPile)
            {
                _buffer >> data;
                event.m_data = static_cast<shared::game::PileType>(data);
            }
            else if (event.m_inputType == shared::game::InputType::ClickSlot)
            {
                shared::game::ClickSlotInputData dataStruct;
                deserializeId(dataStruct.slotId, _buffer);
                deserializeId(dataStruct.playerId, _buffer);
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
        core::event::EventId(events::id::ProvideCard),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::ProvideCardNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_cardId.value());
            _buffer << static_cast<uint8_t>(event.m_rank);
            _buffer << static_cast<uint8_t>(event.m_suit);
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::ProvideCardNetEvent&>(_event);
            deserializeId(event.m_cardId, _buffer);
            deserializeEnum<uint8_t>(event.m_rank, _buffer);
            deserializeEnum<uint8_t>(event.m_suit, _buffer);
        },
        [](){
            return std::make_unique<events::ProvideCardNetEvent>();
        }
    );
    _factoryRef.add(
        core::event::EventId(events::id::DiscardCard),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::DiscardCardNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_rank);
            _buffer << static_cast<uint8_t>(event.m_suit);
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::DiscardCardNetEvent&>(_event);
            deserializeEnum<uint8_t>(event.m_rank, _buffer);
            deserializeEnum<uint8_t>(event.m_suit, _buffer);
        },
        [](){
            return std::make_unique<events::DiscardCardNetEvent>();
        }
    );
    _factoryRef.add(
        core::event::EventId(events::id::MatchCard),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::MatchCardNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_rank);
            _buffer << static_cast<uint8_t>(event.m_suit);
            _buffer << event.m_isMatched;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::MatchCardNetEvent&>(_event);
            deserializeEnum<uint8_t>(event.m_rank, _buffer);
            deserializeEnum<uint8_t>(event.m_suit, _buffer);
            _buffer >> event.m_isMatched;
        },
        [](){
            return std::make_unique<events::MatchCardNetEvent>();
        }
    );
    _factoryRef.add(
        core::event::EventId(events::id::PlayerSlotUpdate),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::PlayerSlotUpdateNetEvent&>(_event);
            _buffer << event.m_playerId.value();
            _buffer << event.m_slotId.value();
            _buffer << event.m_wasAdded;
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::PlayerSlotUpdateNetEvent&>(_event);
            deserializeId(event.m_playerId, _buffer);
            deserializeId(event.m_slotId, _buffer);
            _buffer >> event.m_wasAdded;
        },
        [](){
            return std::make_unique<events::PlayerSlotUpdateNetEvent>();
        }
    );
}

} // namespace cn::net
