#include "server/state/states/LobbyState.hpp"
#include "server/state/StateIds.hpp"

#include "core/event/Dispatcher.hpp"

#include "events/NetworkEvents.hpp"


namespace cn::server::states
{

LobbyState::LobbyState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    m_listenerId = core::event::getNewListenerId();
    CN_LOG("Lobby state..");
}

void LobbyState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::PeerConnectedEvent>(m_listenerId,
            [this](const events::PeerConnectedEvent& _event){
                (void)_event;
                CN_LOG("Player joined..");
                // pop();
                // push(id::MainMenu);
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::PeerConnectedEvent>(m_listenerId);
    }
}

} // namespace cn::server::states
