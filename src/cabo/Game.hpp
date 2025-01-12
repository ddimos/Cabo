#pragma once

#include "core/event/Manager.hpp"
#include "core/state/Manager.hpp"
#include "core/Context.hpp"

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn
{

class Game
{
public:
    Game();
    ~Game() = default;

    void start();

private:
    void init();
    void handleEvents();
    void update(sf::Time _dt);
    void fixedUpdate(sf::Time _dt);
    void draw();

    core::Context m_context;
    sf::RenderWindow m_window;
    sf::Clock m_systemClock;

    TextureHolder m_textureHolder;
    FontHolder m_fontHolder;

    core::event::Manager m_eventManager;
    core::state::Manager m_stateManager;

    bool m_isRunning = false;
};

} // namespace cn
