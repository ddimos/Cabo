#pragma once

#include "core/event/Manager.hpp"
#include "core/state/Manager.hpp"
#include "core/Context.hpp"

#include "shared/net/Manager.hpp"
#include "shared/Types.hpp"

#include "client/player/Manager.hpp"
#include "client/ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client
{

class Client final
{
public:
    Client();

    void start();

private:
    void init();
    void deinit();
    void handleEvents();
    void update(sf::Time _dt);
    void fixedUpdate(sf::Time _dt);
    void draw();

    core::Context m_context;
    sf::RenderWindow m_window;
    sf::Clock m_systemClock;

    TextureHolder m_textureHolder;
    FontHolder m_fontHolder;

    net::Manager m_netManager;
    core::event::Manager m_eventManager;
    core::state::Manager m_stateManager;
    player::Manager m_playerManager;
    shared::Seed m_seed{};
    
    bool m_isRunning = false;
};

} // namespace cn::client
