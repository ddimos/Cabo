#include "client/state/states/JoiningState.hpp"
#include "client/state/StateIds.hpp"

#include "menu/item/Button.hpp"
#include "menu/item/SimpleText.hpp"

#include "net/Manager.hpp"

#include "events/NetworkEvents.hpp"

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::states
{

JoiningState::JoiningState(core::state::Manager& _stateManagerRef)
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
        "Connecting...",
        fontHolderRef.get(FontIds::Main),
        24,
        sf::Color::White
    );
    getContainer(core::object::Container::Type::Menu).add(connectingText);

    auto text = std::make_shared<menu::item::SimpleText>(
        menu::Position{
            .m_position = sf::Vector2f(100.f, 200.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::OFFSET_FROM_CENTER, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        "Back button TODO",
        fontHolderRef.get(FontIds::Main),
        20,
        sf::Color::White
    );
    getContainer(core::object::Container::Type::Menu).add(text);

    m_listenerId = core::event::getNewListenerId();
}

void JoiningState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::PeerConnectedEvent>(m_listenerId,
        [this](const events::PeerConnectedEvent& _event){
            clear();
            push(id::Lobby);
        });
    }
    else
    {
        _dispatcher.unregisterEvent<events::PeerConnectedEvent>(m_listenerId);
    }
}

} // namespace cn::client::states
