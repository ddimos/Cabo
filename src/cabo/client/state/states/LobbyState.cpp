#include "client/state/states/LobbyState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "client/menu/item/Button.hpp"
#include "client/menu/item/SimpleText.hpp"

#include "client/player/Manager.hpp"

#include "core/event/Dispatcher.hpp"

#include "shared/events/InputEvents.hpp"
#include "shared/events/NetworkEvents.hpp"
#include "shared/game/Constants.hpp"
#include "shared/net/Manager.hpp"
#include "shared/player/Types.hpp"

#include "LaunchTarget.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::states
{

LobbyState::LobbyState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(core::object::Container::Type::Menu);
    
    auto& windowRef = getContext().get<sf::RenderWindow>();
    auto& textureHolderRef = getContext().get<TextureHolder>();
    auto& fontHolderRef = getContext().get<FontHolder>();
    auto& netManRef = getContext().get<net::Manager>();
    auto& playerManRef = getContext().get<player::Manager>();

    menu::Position playerListPos;
    playerListPos.m_position = sf::Vector2f(0.f, -300.f);
    playerListPos.m_parentSize = sf::Vector2f(windowRef.getSize());
    playerListPos.m_specPositionX = menu::Position::Special::CENTER_ALLIGNED;
    playerListPos.m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER;
    sf::Vector2f globalPos = playerListPos.calculateGlobalPos(sf::Vector2f(300.f, 400.f));

    m_players.reserve(shared::game::MaxNumberOfPlayers);
    for (short unsigned i = 0; i < shared::game::MaxNumberOfPlayers; ++i)
    {
        auto playerName = std::make_shared<menu::item::SimpleText>(
            menu::Position{
                .m_position = sf::Vector2f(0.f, 40.f * i), .m_parentPos = globalPos, .m_parentSize = sf::Vector2f(300.f, 400.f),
                .m_specPositionX = menu::Position::Special::OFFSET_FROM_START, .m_specPositionY = menu::Position::Special::OFFSET_FROM_START
            },
            "",
            fontHolderRef.get(FontIds::Main),
            24,
            sf::Color::White
        );
        playerName->setActivationOption(core::object::Object::ActivationOption::Manually);
        getContainer(core::object::Container::Type::Menu).add(playerName);

        auto status = std::make_shared<menu::item::SimpleText>(
            menu::Position{
                .m_position = sf::Vector2f(100.f, 40.f * i), .m_parentPos = globalPos, .m_parentSize = sf::Vector2f(300.f, 400.f),
                .m_specPositionX = menu::Position::Special::OFFSET_FROM_END, .m_specPositionY = menu::Position::Special::OFFSET_FROM_START
            },
            "Not ready",
            fontHolderRef.get(FontIds::Main),
            24,
            sf::Color::White
        );
        status->setActivationOption(core::object::Object::ActivationOption::Manually);
        getContainer(core::object::Container::Type::Menu).add(status);

        m_players.emplace_back(PlayerItem{ .m_nameTextRef = *playerName, .m_statusTextRef = *status });
    }

    auto startButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, 100.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        textureHolderRef.get(TextureIds::MainMenuStartButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [&netManRef, &playerManRef](){
            events::PlayerReadyStatusUpdateNetEvent event(playerManRef.getLocalPlayerId(), true);
            netManRef.send(event);
        },
        sf::Mouse::Button::Left
    );
    getContainer(core::object::Container::Type::Menu).add(startButton);

    for (auto& player : playerManRef.getPlayers())
        addPlayer(player.id, player.name);

    m_listenerId = core::event::getNewListenerId();
}

void LobbyState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::StartGameNetEvent>(m_listenerId,
            [this](const events::StartGameNetEvent& _event){
                (void)_event;
                pop();
                push(id::Game);
            }
        );
        _dispatcher.registerEvent<events::PlayerReadyStatusUpdateNetEvent>(m_listenerId,
            [this](const events::PlayerReadyStatusUpdateNetEvent& _event){
                if (!hasPlayer(_event.m_id))
                {
                    addPlayer(_event.m_id, getContext().get<player::Manager>().getPlayer(_event.m_id)->name);
                }
                updatePlayer(_event.m_id, _event.m_ready);
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::StartGameNetEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::PlayerReadyStatusUpdateNetEvent>(m_listenerId);
    }
}

bool LobbyState::hasPlayer(PlayerId _playerId) const
{
    for (const auto& player : m_players)
    {
        if (player.m_playerId == _playerId)
            return true;
    }
    return false;
}

void LobbyState::addPlayer(PlayerId _playerId, const std::string& _name)
{
    for (auto& player : m_players)
    {
        if (player.m_playerId != PlayerIdInvalid)
            continue;
        
        player.m_playerId = _playerId;
        player.m_nameTextRef.updateText(_name);
        player.m_nameTextRef.requestActivated();
        player.m_statusTextRef.requestActivated();
        return;
    }
    CN_ASSERT_FRM(false, "The player with id {} wasn't found", _playerId);
}

void LobbyState::updatePlayer(PlayerId _playerId, bool _isReady)
{
    for (auto& player : m_players)
    {
        if (player.m_playerId != _playerId)
            continue;

        player.m_statusTextRef.updateText(_isReady ? "Ready" : "Not ready");
        return;
    }

    CN_ASSERT_FRM(false, "The player with id {} wasn't found", _playerId);
}

} // namespace cn::client::states
