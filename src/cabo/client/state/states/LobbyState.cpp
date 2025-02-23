#include "client/state/states/LobbyState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "client/menu/item/Button.hpp"
#include "client/menu/item/SimpleText.hpp"

#include "core/event/Dispatcher.hpp"

#include "shared/events/InputEvents.hpp"
#include "shared/events/NetworkEvents.hpp"
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

    auto connectingText = std::make_shared<menu::item::SimpleText>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, 0.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::CENTER_ALLIGNED
        },
        "Lobby...",
        fontHolderRef.get(FontIds::Main),
        24,
        sf::Color::White
    );
    getContainer(core::object::Container::Type::Menu).add(connectingText);

    auto startButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, 100.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        textureHolderRef.get(TextureIds::MainMenuStartButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [&netManRef](){
            events::PlayerReadyNetEvent event;
            netManRef.send(event);
        },
        sf::Mouse::Button::Left
    );
    getContainer(core::object::Container::Type::Menu).add(startButton);

    events::PlayerInfoUpdateNetEvent event({ Player{ .name = "Player Name"} });
    netManRef.send(event);

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
    }
    else
    {
        _dispatcher.unregisterEvent<events::StartGameNetEvent>(m_listenerId);
    }
}

} // namespace cn::client::states
