#include "client/state/states/LobbyState.hpp"
#include "client/state/StateIds.hpp"

#include "core/event/Dispatcher.hpp"
#include "events/SystemEvents.hpp"

#include "LaunchTarget.hpp"
#include "menu/item/SimpleText.hpp"

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::states
{

LobbyState::LobbyState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
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
