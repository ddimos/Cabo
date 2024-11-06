#pragma once

#include "state/StateManager.hpp"
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
    void update();
    void fixedUpdate(sf::Time _dt);
    void draw();

    sf::RenderWindow m_window;
    sf::Clock m_systemClock;

    TextureHolder m_textureHolder;
    FontHolder m_fontHolder;

    state::StateManager m_stateManager;

    bool m_isRunning = false;
};

} // namespace cn
