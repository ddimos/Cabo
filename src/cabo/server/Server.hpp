#pragma once

#include "core/event/Manager.hpp"
#include "core/state/Manager.hpp"
#include "core/Context.hpp"

#include "net/Manager.hpp"

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

    bool m_isRunning = false;
};
    
} // namespace cn::server
