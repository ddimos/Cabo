#include "client/state/states/GameState.hpp"
#include "client/state/StateIds.hpp"

#include "client/player/Manager.hpp"

#include "client/game/Anchor.hpp"
#include "client/game/Card.hpp"
#include "client/game/Deck.hpp"
#include "client/game/Discard.hpp"
#include "client/game/Participant.hpp"
#include "client/game/PrivateZone.hpp"
#include "client/game/SpriteSheet.hpp"

#include "core/event/Dispatcher.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"
#include "shared/Types.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include "core/Log.hpp"

namespace
{
constexpr cn::core::object::Container::Id GameContainerId = 1;
constexpr cn::core::object::Container::Id MenuContainerId = 2;
} // namespace

namespace cn::client::states
{

GameState::GameState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(MenuContainerId);
    createSortedContainer(GameContainerId, 
        [](const core::object::Object& _left, const core::object::Object& _right){
            const auto& left = static_cast<const shared::game::object::Object&>(_left);
            const auto& right = static_cast<const shared::game::object::Object&>(_right);
            return left.getZIndex() < right.getZIndex();
        }
    );

    auto& playerManagerRef = getContext().get<player::Manager>();

    // On the clients we don't use the add method for the next controllers.
    // TODO think how to rewrite controllers not to expose it 
    m_flipController = std::make_unique<shared::game::controller::Flippable>();
    m_grabController = std::make_unique<shared::game::controller::Grabbable>();
    // For this, we use the method
    m_privateZoneViewableController = std::make_unique<shared::game::controller::PrivateZoneViewable>(
        [&playerManagerRef](shared::game::component::PrivateZoneViewable& _component){
            if (_component.isHidden() && !_component.isHiddenInZoneOfPlayer(playerManagerRef.getLocalPlayerId()))
            {
                auto& card = static_cast<shared::game::object::Card&>(_component.getParent());
                card.setValue(shared::game::object::Card::Value{});
            }
        }
    );

    m_inputController = std::make_unique<game::InputController>(getContext(), 
        [this, &playerManagerRef](sf::Vector2f _pos){
            m_board->participantMoves(playerManagerRef.getLocalPlayerId(), _pos);
        },
        [this, &playerManagerRef](sf::Vector2f _pos){
            // pre release the object to avoid visual gliches with high ping
            m_board->getParticipant(playerManagerRef.getLocalPlayerId())->setObject(nullptr);
        }
    );

    m_board = std::make_unique<shared::game::Board>(playerManagerRef.getPlayers(),
        [this](shared::game::object::Id _id){
            auto card = std::make_shared<game::Card>(getContext(), _id);
            getContainer(GameContainerId).add(card);
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
        [&playerManagerRef, this](shared::game::object::Id _id, PlayerId _playerId){
            auto participant = std::make_shared<game::Participant>(getContext(), _id, _playerId, _playerId == playerManagerRef.getLocalPlayerId());
            getContainer(GameContainerId).add(participant);
            return participant.get();
        },
        [this](shared::game::object::Id _id, PlayerId _playerId){
            auto zone = std::make_shared<game::PrivateZone>(_id, _playerId);
            getContainer(GameContainerId).add(zone);
            m_privateZoneViewableController->addPrivateZone(*zone);
            return zone.get();
        }
    );

    m_listenerId = core::event::getNewListenerId();

    // We don't know the values on the client yet
    std::vector<game::Card::Value> cardValues;
    m_board->start(cardValues);
}

void GameState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::FinishGameNetEvent>(m_listenerId,
            [this](const events::FinishGameNetEvent& _event){
                pop();
                push(id::Finish);
            }
        );
        _dispatcher.registerEvent<events::ServerCommandNetEvent>(m_listenerId, // TODO I probably should move this code somewhere else 
            [this](const events::ServerCommandNetEvent& _event){
                if (_event.m_type == shared::game::ServerCommandType::PlayerInteractsWithCard)
                {
                    const auto& data = std::get<shared::game::PlayerInteractsWithCardData>(_event.m_data);
                    if (data.type == shared::game::PlayerInteractsWithCardData::Type::Grabs)
                    {
                        auto* card = m_board->getCard(data.cardId);
                        m_grabController->grabObject(data.playerId, card->getGrabbableComponent());
                        m_board->participantGrabs(data.playerId, data.cardId, data.pos);
                    }
                    else if (data.type == shared::game::PlayerInteractsWithCardData::Type::Releases)
                    {
                        auto* card = m_board->getCard(data.cardId);
                        m_grabController->releaseObject(data.playerId, card->getGrabbableComponent());
                        m_board->participantReleases(data.playerId, data.cardId, data.pos);
                        static_cast<game::Card*>(card)->startTransit(data.pos);
                    }
                    else if (data.type == shared::game::PlayerInteractsWithCardData::Type::TurnsDown)
                    {
                        auto* card = m_board->getCard(data.cardId);
                        m_flipController->flipObject(card->getFlippableComponent());
                        m_board->participantTurnsDown(data.playerId, data.cardId, data.pos);
                        card->setValue(shared::game::object::Card::Value{});

                        // TODO start flipping
                    }
                    else if (data.type == shared::game::PlayerInteractsWithCardData::Type::TurnsUp)
                    {
                        auto* card = m_board->getCard(data.cardId);
                        m_flipController->flipObject(card->getFlippableComponent());
                        m_board->participantTurnsUp(data.playerId, data.cardId, data.pos);
                    }
                }
                else if (_event.m_type == shared::game::ServerCommandType::PlayerMoves)
                {
                    const auto& data = std::get<shared::game::PlayerMovesData>(_event.m_data);
                    m_board->participantMoves(data.playerId, data.pos);
                }
                else if (_event.m_type == shared::game::ServerCommandType::ProvideCardValue)
                {
                    const auto& data = std::get<shared::game::ProvideCardValueData>(_event.m_data);
                    m_board->getCard(data.cardId)->setValue(data.value);
                }
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::FinishGameNetEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::ServerCommandNetEvent>(m_listenerId);
    }

    m_inputController->registerEvents(_dispatcher, _isBeingRegistered);
    // m_board->registerEvents(_dispatcher, _isBeingRegistered);
}

core::state::Return GameState::onUpdate(sf::Time _dt)
{
    // m_board->update(_dt);
    m_privateZoneViewableController->update();
    return core::state::Return::Break;
}

} // namespace cn::client::states
