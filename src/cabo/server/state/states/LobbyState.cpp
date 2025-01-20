#include "server/state/states/LobbyState.hpp"
#include "server/state/StateIds.hpp"

#include "core/event/Dispatcher.hpp"

#include "events/ConnectionEvents.hpp"
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
        _dispatcher.registerEvent<events::PlayerInfoUpdateEvent>(m_listenerId,
            [this](const events::PlayerInfoUpdateEvent& _event){
                (void)_event;
                CN_LOG_FRM("Player info.. {}", _event.m_name);
                // pop();
                // push(id::MainMenu);
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::PeerConnectedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::PlayerInfoUpdateEvent>(m_listenerId);
    }
}

} // namespace cn::server::states
