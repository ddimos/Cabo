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
                // TODO send the update to the peer
            }
        );
        _dispatcher.registerEvent<events::PlayerReadyStatusUpdateNetEvent>(m_listenerId,
            [this](const events::PlayerReadyStatusUpdateNetEvent& _event){
                (void)_event;
                CN_LOG_FRM("Player ready.. {}", _event.m_senderPeerId);
                CN_ASSERT(_event.m_ready); // TODO to implement m_ready == false;
                m_players.erase(_event.m_senderPeerId);

                auto& netManRef = getContext().get<net::Manager>();
                events::PlayerReadyStatusUpdateNetEvent event(_event.m_senderPeerId, true);
                netManRef.send(event);

                if (m_players.empty())
                {
                    CN_LOG("All players ready..");
                    events::StartGameNetEvent event;
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
        _dispatcher.unregisterEvent<events::PlayerReadyStatusUpdateNetEvent>(m_listenerId);
    }
}

} // namespace cn::server::states
