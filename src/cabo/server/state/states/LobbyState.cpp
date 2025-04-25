#include "server/state/states/LobbyState.hpp"
#include "server/state/StateIds.hpp"

#include "server/player/Manager.hpp"

#include "core/event/Dispatcher.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"
#include "shared/Types.hpp"

#include <ctime>

namespace cn::server::states
{

LobbyState::LobbyState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    uint32_t seed = static_cast<uint32_t>(std::time(nullptr));
    getContext().get<shared::Seed>().seed = seed;

    m_listenerId = core::event::getNewListenerId();
    CN_LOG_FRM("Lobby state.., seed {}", seed);
}

void LobbyState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::PlayerPresenceChangedEvent>(m_listenerId,
            [this](const events::PlayerPresenceChangedEvent& _event){

                if (_event.m_joined)
                {
                    m_players.emplace(_event.m_id, false);
                    m_startGame = false;
                }
                else
                {
                    m_players.erase(_event.m_id);
                    startGameIfAllPlayersReady();
                }

                events::PlayerReadyStatusUpdateNetEvent event(m_players);
                getContext().get<net::Manager>().send(event);
            }
        );
        _dispatcher.registerEvent<events::PlayerReadyStatusUpdateNetEvent>(m_listenerId,
            [this](const events::PlayerReadyStatusUpdateNetEvent& _event){
                CN_ASSERT(_event.m_players.size() == 1);
                CN_ASSERT(_event.m_players.begin()->second); // TODO to implement m_ready == false;
                
                CN_LOG_FRM("Player is ready.. {}", _event.m_senderPeerId);

                auto it = m_players.find(_event.m_players.begin()->first);
                it->second = true;

                auto& netManRef = getContext().get<net::Manager>();
                events::PlayerReadyStatusUpdateNetEvent event(m_players);
                netManRef.send(event);

                startGameIfAllPlayersReady();
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::PlayerPresenceChangedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::PlayerReadyStatusUpdateNetEvent>(m_listenerId);
    }
}

core::state::Return LobbyState::onUpdate(sf::Time _dt)
{
    if (m_startGame)
    {
        m_timeToWaitDt -= _dt;

        if (m_timeToWaitDt.asSeconds() <= 0.f)
        {
            CN_LOG("Start game..");

            events::StartGameNetEvent event(getContext().get<shared::Seed>().seed);
            getContext().get<net::Manager>().send(event);
        
            pop();
            push(id::Game);
        }
    }

    return core::state::Return::Break;
}

void LobbyState::startGameIfAllPlayersReady()
{
    bool allPlayersReady = true;
    for (const auto& [id, ready] : m_players)
    {
        if (!ready)
        {
            allPlayersReady = false;
            break;
        }
    }

    if (allPlayersReady)
    {
        CN_LOG("All players ready..");
        m_startGame = true;
        m_timeToWaitDt = m_timeToWait;
    }
}

} // namespace cn::server::states
