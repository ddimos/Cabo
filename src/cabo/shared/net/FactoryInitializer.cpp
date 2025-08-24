#include "shared/net/FactoryInitializer.hpp"
#include "shared/net/Factory.hpp"

#include "shared/events/NetworkEvents.hpp"

#include "core/Assert.hpp"

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
        core::event::EventId(events::id::RemotePlayerInput),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::RemotePlayerInputNetEvent&>(_event);
            _buffer << event.m_playerId.value();
            _buffer << static_cast<uint8_t>(event.m_type);

            if (event.m_type == shared::game::PlayerInputType::Grab
                || event.m_type == shared::game::PlayerInputType::Release
                || event.m_type == shared::game::PlayerInputType::Flip
                || event.m_type == shared::game::PlayerInputType::Move)
            {
                const auto& data = std::get<sf::Vector2f>(event.m_data);
                _buffer << data.x;
                _buffer << data.y;
            }
            else
            {
                CN_ASSERT(false);
            }
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::RemotePlayerInputNetEvent&>(_event);
            deserializeId(event.m_playerId, _buffer);
            deserializeEnum<uint8_t>(event.m_type, _buffer);
            
            if (event.m_type == shared::game::PlayerInputType::Grab 
                || event.m_type == shared::game::PlayerInputType::Release
                || event.m_type == shared::game::PlayerInputType::Flip
                || event.m_type == shared::game::PlayerInputType::Move)
            {
                sf::Vector2f data;
                _buffer >> data.x;
                _buffer >> data.y;
                event.m_data = data;
            }
            else
            {
                CN_ASSERT(false);
            }
        },
        [](){
            return std::make_unique<events::RemotePlayerInputNetEvent>();
        }
    );
    _factoryRef.add(
        core::event::EventId(events::id::ServerCommand),
        [](const core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<const events::ServerCommandNetEvent&>(_event);
            _buffer << static_cast<uint8_t>(event.m_type);
            if (event.m_type == shared::game::ServerCommandType::PlayerInteractsWithCard)
            {
                const auto& data = std::get<shared::game::PlayerInteractsWithCardData>(event.m_data);
                _buffer << data.playerId.value();
                _buffer << data.cardId.value();
                _buffer << data.pos.x;
                _buffer << data.pos.y;
                _buffer << static_cast<uint8_t>(data.type);
            }
            else if (event.m_type == shared::game::ServerCommandType::PlayerMoves)
            {
                const auto& data = std::get<shared::game::PlayerMovesData>(event.m_data);
                _buffer << data.playerId.value();
                _buffer << data.pos.x;
                _buffer << data.pos.y;
            }
            else if (event.m_type == shared::game::ServerCommandType::ProvideCardValue)
            {
                const auto& data = std::get<shared::game::ProvideCardValueData>(event.m_data);
                _buffer << data.cardId.value();
                _buffer << data.value.value();
            }
            else
            {
                CN_ASSERT(false);
            }
        },
        [](core::event::Event& _event, nsf::Buffer& _buffer){
            auto& event = static_cast<events::ServerCommandNetEvent&>(_event);
            deserializeEnum<uint8_t>(event.m_type, _buffer);
            if (event.m_type == shared::game::ServerCommandType::PlayerInteractsWithCard)
            {
                shared::game::PlayerInteractsWithCardData data;
                deserializeId(data.playerId, _buffer);
                deserializeId(data.cardId, _buffer);
                _buffer >> data.pos.x;
                _buffer >> data.pos.y;
                deserializeEnum<uint8_t>(data.type, _buffer);
                event.m_data = data;
            }
            else if (event.m_type == shared::game::ServerCommandType::PlayerMoves)
            {
                shared::game::PlayerMovesData data;
                deserializeId(data.playerId, _buffer);
                _buffer >> data.pos.x;
                _buffer >> data.pos.y;
                event.m_data = data;
            }
            else if (event.m_type == shared::game::ServerCommandType::ProvideCardValue)
            {
                shared::game::ProvideCardValueData data;
                deserializeId(data.cardId, _buffer);
                deserializeId(data.value, _buffer);
                event.m_data = data;
            }
            else
            {
                CN_ASSERT(false);
            }
        },
        [](){
            return std::make_unique<events::ServerCommandNetEvent>();
        }
    );
}

} // namespace cn::net
