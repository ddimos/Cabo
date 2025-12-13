#include "server/state/states/GameState.hpp"
#include "server/state/StateIds.hpp"

#include "server/player/Manager.hpp"

#include "server/game/Card.hpp"
#include "server/game/CountableButton.hpp"
#include "server/game/Deck.hpp"
#include "server/game/Discard.hpp"
#include "server/game/Participant.hpp"
#include "server/game/PrivateZone.hpp"

#include "core/event/Dispatcher.hpp"

#include "shared/events/NetworkEvents.hpp"
#include "shared/events/GameEvents.hpp"
#include "shared/net/Manager.hpp"
#include "shared/Types.hpp"
#include "shared/game/object/Object.hpp"

#include "core/Log.hpp"

#include <ctime>

namespace
{
constexpr cn::core::object::Container::Id GameContainerId = 1;
} // namespace

namespace cn::server::states
{

GameState::GameState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createSortedContainer(GameContainerId, 
        [](const core::object::Object& _left, const core::object::Object& _right){
            const auto& left = static_cast<const shared::game::object::Object&>(_left);
            const auto& right = static_cast<const shared::game::object::Object&>(_right);
            return left.getZIndex() < right.getZIndex();
        }
    );

    auto& netManagerRef = getContext().get<net::Manager>();
    auto& playerManagerRef = getContext().get<player::Manager>();
    unsigned seed = getContext().get<shared::Seed>().seed;

    m_flipController = std::make_unique<shared::game::controller::Flippable>();
    m_grabController = std::make_unique<shared::game::controller::Grabbable>();
    m_privateZoneViewableController = std::make_unique<shared::game::controller::PrivateZoneViewable>(
        [&](shared::game::component::PrivateZoneViewable& _component){
            // CN_LOG("PRIVATE ZONE");
            if (!_component.isHidden())
            {
                auto& card = static_cast<shared::game::object::Card&>(_component.getParent());
                events::ServerCommandNetEvent event(
                    shared::game::ServerCommandType::ProvideCardValue,
                    shared::game::ProvideCardValueData{
                        .cardId = card.getId(),
                        .value = card.getValue()
                    }
                );
                netManagerRef.send(event);
            }
        }
    );

    m_inputController = std::make_unique<game::InputController>(getContext(), 
        [this, &netManagerRef](const events::RemotePlayerInputNetEvent& _event){
            if (_event.m_type == shared::game::PlayerInputType::Click)
            {
                const auto& data = std::get<sf::Vector2f>(_event.m_data);
                auto* object = m_board->participantClicks(_event.m_playerId, data);
                if (!object)
                    return;
                CN_LOG_I_FRM("Click {} {}", data.x, data.y);
                
                events::ServerCommandNetEvent event(
                    shared::game::ServerCommandType::PlayerClicksOnButton,
                    shared::game::PlayerClicksOnButtonData{ .playerId = _event.m_playerId, .id = object->getId() }
                );
                netManagerRef.send(event);
            }
            else if (_event.m_type == shared::game::PlayerInputType::Grab)
            {
                const auto& data = std::get<sf::Vector2f>(_event.m_data);
                m_board->participantMoves(_event.m_playerId, data);
                auto* component = m_grabController->findObjectToGrab(_event.m_playerId, data);
                if (!component)
                    return;

                m_grabController->grabObject(_event.m_playerId, *component);
                auto& object = component->getParent();
                m_board->participantGrabs(_event.m_playerId, object.getId(), data);

                events::ServerCommandNetEvent event(
                    shared::game::ServerCommandType::PlayerInteractsWithCard,
                    shared::game::PlayerInteractsWithCardData{ .playerId = _event.m_playerId, .cardId = object.getId(), .pos = data, .type = shared::game::PlayerInteractsWithCardData::Type::Grabs }
                );
                netManagerRef.send(event);
            }
            else if (_event.m_type == shared::game::PlayerInputType::Release)
            {
                const auto& data = std::get<sf::Vector2f>(_event.m_data);
                m_board->participantMoves(_event.m_playerId, data);
                auto* component = m_grabController->findObjectToRelease(_event.m_playerId, data);
                CN_LOG_I_FRM("Release {} {}", data.x, data.y);
                if (!component)
                    return;

                m_grabController->releaseObject(_event.m_playerId, *component);
                auto& object = component->getParent();
                m_board->participantReleases(_event.m_playerId, object.getId(), data);

                events::ServerCommandNetEvent event(
                    shared::game::ServerCommandType::PlayerInteractsWithCard,
                    shared::game::PlayerInteractsWithCardData{ .playerId = _event.m_playerId, .cardId = object.getId(), .pos = data, .type = shared::game::PlayerInteractsWithCardData::Type::Releases }
                );
                netManagerRef.send(event);
            }
            else if (_event.m_type == shared::game::PlayerInputType::Flip)
            {
                const auto& data = std::get<sf::Vector2f>(_event.m_data);
                m_board->participantMoves(_event.m_playerId, data);

                auto* component = m_flipController->findObjectToFlip(data);
                if (!component)
                    return;
                
                m_flipController->flipObject(*component);
                auto& card = static_cast<shared::game::object::Card&>(component->getParent());               
                if (component->isFaceUp())
                    m_board->participantTurnsUp(_event.m_playerId, card.getId(), data);
                else
                    m_board->participantTurnsDown(_event.m_playerId, card.getId(), data);

                {
                    events::ServerCommandNetEvent event(
                        shared::game::ServerCommandType::PlayerInteractsWithCard,
                        shared::game::PlayerInteractsWithCardData{ 
                            .playerId = _event.m_playerId,
                            .cardId = card.getId(),
                            .pos = data,
                            .type = component->isFaceUp()
                                ? shared::game::PlayerInteractsWithCardData::Type::TurnsUp
                                : shared::game::PlayerInteractsWithCardData::Type::TurnsDown
                        }
                    );
                    netManagerRef.send(event);
                }
                {
                    events::ServerCommandNetEvent event(
                        shared::game::ServerCommandType::ProvideCardValue,
                        shared::game::ProvideCardValueData{
                            .cardId = card.getId(),
                            .value = card.getValue()
                        }
                    );
                    CN_LOG_I_FRM("Card {} value {}", card.getId().value(), card.getValue().value());
                    if (card.getPrivateZoneViewableComponent().isHiddenInZoneOfPlayer(_event.m_playerId))
                    {
                        netManagerRef.send(event, nsf::PeerID(_event.m_playerId.value()));
                    }
                    else if (card.getPrivateZoneViewableComponent().isHidden())
                    {
                        netManagerRef.send(event, nsf::PeerID(card.getPrivateZoneViewableComponent().getPrivateZone()->getOwnerId().value()));
                    }
                    else
                    {
                        netManagerRef.send(event);
                    }
                }
            }
            else if (_event.m_type == shared::game::PlayerInputType::Move)
            {
                const auto& data = std::get<sf::Vector2f>(_event.m_data);
                m_board->participantMoves(_event.m_playerId, data);

                events::ServerCommandNetEvent event(
                    shared::game::ServerCommandType::PlayerMoves,
                    shared::game::PlayerMovesData{ .playerId = _event.m_playerId, .pos = data }
                );
                netManagerRef.send(event, nsf::MessageInfo::Type::EXCLUDE_BRODCAST, false, nsf::PeerID{ _event.m_playerId.value() });
            }
        }
    );

    m_board = std::make_unique<shared::game::Board>(playerManagerRef.getPlayers(),
        [this](shared::game::object::Id _id){
            auto card = std::make_shared<game::Card>(_id);
            getContainer(GameContainerId).add(card);
            m_flipController->add(card->getFlippableComponent());
            m_grabController->add(card->getGrabbableComponent());
            m_privateZoneViewableController->add(card->getPrivateZoneViewableComponent());
            return card.get();
        },
        [this](shared::game::object::Id _id){
            auto deck = std::make_shared<game::Deck>(_id, getContext().get<shared::Seed>().seed);
            getContainer(GameContainerId).add(deck);
            return deck.get();
        },
        [this](shared::game::object::Id _id){
            auto discard = std::make_shared<game::Discard>(_id);
            getContainer(GameContainerId).add(discard);
            return discard.get();
        },
        [this](shared::game::object::Id _id, PlayerId _playerId){
            auto participant = std::make_shared<game::Participant>(_id, _playerId);
            getContainer(GameContainerId).add(participant);
            return participant.get();
        },
        [this](shared::game::object::Id _id, PlayerId _playerId){
            auto zone = std::make_shared<game::PrivateZone>(_id, _playerId);
            getContainer(GameContainerId).add(zone);
            m_privateZoneViewableController->addPrivateZone(*zone);
            return zone.get();
        },
        [this](shared::game::object::Id _id, shared::game::TableButtonType _type, unsigned _numberOfOPlayersToClick){
            auto button = std::make_shared<game::CountableButton>(_id, _numberOfOPlayersToClick, [](PlayerId){}, [](){});
            getContainer(GameContainerId).add(button);
            return button.get();
        }
    );

    m_listenerId = core::event::getNewListenerId();

    uint32_t cardValueSeed = static_cast<uint32_t>(std::time(nullptr));
    m_cardValueGenerator.init(cardValueSeed);

    auto cardValuesRaw = m_cardValueGenerator.generateSequenceNoRepeats(shared::game::StandartDeckSize);
    std::vector<game::Card::Value> cardValues;
    cardValues.reserve(cardValuesRaw.size());
    for (auto value : cardValuesRaw)
        cardValues.push_back(game::Card::Value(static_cast<uint8_t>(value)));
    m_board->start(cardValues);

    CN_LOG_I("Game state..");
}

void GameState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    m_inputController->registerEvents(_dispatcher, _isBeingRegistered);
    // m_board->registerEvents(_dispatcher, _isBeingRegistered);
}

core::state::Return GameState::onUpdate(sf::Time _dt)
{
    m_inputController->update();
    m_privateZoneViewableController->update();
    m_board->update(_dt);

    // if (m_board->isFinished())
    // {
    //     events::FinishGameNetEvent event;
    //     getContext().get<net::Manager>().send(event);
    //     pop();
    //     push(id::Finish);
    // }
    return core::state::Return::Break;
}

} // namespace cn::server::states
