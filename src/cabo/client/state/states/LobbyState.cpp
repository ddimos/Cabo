#include "client/state/states/LobbyState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "core/event/Dispatcher.hpp"
#include "events/InputEvents.hpp"
#include "events/NetworkEvents.hpp"

#include "net/Manager.hpp"

#include "LaunchTarget.hpp"
#include "menu/item/SimpleText.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::states
{

LobbyState::LobbyState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(core::object::Container::Type::Menu);
    
    auto& windowRef = getContext().get<sf::RenderWindow>();
    auto& fontHolderRef = getContext().get<FontHolder>();

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

    events::PlayerInfoUpdateEvent event("Player name");
    auto& netMan = getContext().get<net::Manager>();
    netMan.send(true, event);

    m_listenerId = core::event::getNewListenerId();
}

void LobbyState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::KeyReleasedEvent>(m_listenerId,
            [this](const events::KeyReleasedEvent& _event){
                (void)_event;
                pop();
                push(id::MainMenu);
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::KeyReleasedEvent>(m_listenerId);
    }
}

} // namespace cn::client::states
