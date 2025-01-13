#include "server/state/states/LobbyState.hpp"
#include "server/state/StateIds.hpp"

#include "core/event/Dispatcher.hpp"
#include "events/SystemEvents.hpp"

#include "LaunchTarget.hpp"
// #include "menu/item/SimpleText.hpp"

// #include "ResourceIds.hpp"

// #include <SFML/Graphics/RenderWindow.hpp>

namespace cn::server::states
{

LobbyState::LobbyState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    // if CN_IF_CLIENT
    {
        // createContainer(core::object::Container::Type::Menu);


        // auto text = std::make_shared<menu::item::SimpleText>(
        //     menu::Position{
        //         .m_position = sf::Vector2f(0.f, -45.f), .m_parentSize = sf::Vector2f(getContext().windowRef.getSize()),
        //         .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        //     },
        //     "Finish",
        //     getContext().fontHolderRef.get(FontIds::Main),
        //     20,
        //     sf::Color::White
        // );
        // getContainer(core::object::Container::Type::Menu).add(text);
    }

    m_listenerId = core::event::getNewListenerId();
}

void LobbyState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::KeyReleasedEvent>(m_listenerId,
            [this](const events::KeyReleasedEvent& _event){
                (void)_event;
                // pop();
                // push(id::MainMenu);
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::KeyReleasedEvent>(m_listenerId);
    }
}

} // namespace cn::server::states
