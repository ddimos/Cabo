#pragma once

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
    void fixedUpdate(float _dt);
    void draw();

    sf::RenderWindow m_window;
    sf::Clock m_systemClock;

    TextureHolder m_textureHolder;
    FontHolder m_fontHolder;

    bool m_isRunning = false;
};

} // namespace cn
