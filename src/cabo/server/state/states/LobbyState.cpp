#include "server/state/states/LobbyState.hpp"
#include "server/state/StateIds.hpp"

#include "core/event/Dispatcher.hpp"

#include "shared/events/ConnectionEvents.hpp"
#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"

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
                m_players.insert(_event.m_peerId);
            }
        );
        _dispatcher.registerEvent<events::PlayerReadyEvent>(m_listenerId,
            [this](const events::PlayerReadyEvent& _event){
                (void)_event;
                CN_LOG_FRM("Player ready.. {}", _event.m_senderPeerId);
                m_players.erase(_event.m_senderPeerId);

                if (m_players.empty())
                {
                    CN_LOG("All players ready..");
                    events::StartGameEvent event;
                    auto& netManRef = getContext().get<net::Manager>();
                    netManRef.send(event);
                
                    pop();
                    push(id::Game);
                }
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::PeerConnectedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::PlayerReadyEvent>(m_listenerId);
    }
}

} // namespace cn::server::states
