#pragma once

#include "core/event/Types.hpp"
#include "core/state/State.hpp"

#include "shared/player/Types.hpp"

#include <nsf/Types.hpp>
#include <SFML/System/Time.hpp>
#include <unordered_map>

namespace cn::server::states
{

class LobbyState final : public core::state::State
{
public:
    LobbyState(core::state::Manager& _stateManagerRef);

private:
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;
    core::state::Return onUpdate(sf::Time _dt) override;
    
    void startGameIfAllPlayersReady();

    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
    std::unordered_map<PlayerId, bool> m_players;

    bool m_startGame = false;
    const sf::Time m_timeToWait = sf::seconds(2.f);
    sf::Time m_timeToWaitDt = {};
};

} // namespace cn::server::states
