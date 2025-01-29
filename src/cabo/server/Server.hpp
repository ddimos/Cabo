#pragma once

#include "core/event/Manager.hpp"
#include "core/state/Manager.hpp"
#include "core/Context.hpp"

#include "shared/net/Manager.hpp"

#include "server/player/Manager.hpp"

namespace cn::server
{

class Server final
{
public:
    Server();

    void start();

private:
    void init();
    void handleEvents();
    void update(sf::Time _dt);
    void fixedUpdate(sf::Time _dt);

    core::Context m_context;
    sf::Clock m_systemClock;

    net::Manager m_netManager;
    core::event::Manager m_eventManager;
    core::state::Manager m_stateManager;
    player::Manager m_playerManager;

    bool m_isRunning = false;
};
    
} // namespace cn::server
