#include "server/state/states/LobbyState.hpp"
#include "server/state/StateIds.hpp"

#include "core/event/Dispatcher.hpp"

#include "events/ConnectionEvents.hpp"
#include "events/NetworkEvents.hpp"

#include "net/Manager.hpp"

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
                m_players.insert(_event.m_peerId);
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
        _dispatcher.registerEvent<events::PlayerReadyEvent>(m_listenerId,
            [this](const events::PlayerReadyEvent& _event){
                (void)_event;
                CN_LOG_FRM("Player ready.. {}", _event.m_peerId);
                m_players.erase(_event.m_peerId);

                if (m_players.empty())
                {
                    CN_LOG("All players ready..");
                    events::StartGameEvent event;
                    auto& netManRef = getContext().get<net::Manager>();
                    netManRef.send(true, event);
                }
                pop();
                push(id::Game);
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::PeerConnectedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::PlayerInfoUpdateEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::PlayerReadyEvent>(m_listenerId);
    }
}

} // namespace cn::server::states
