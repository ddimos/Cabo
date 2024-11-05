#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

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
    void render();

    sf::RenderWindow m_window;
    sf::Clock m_systemClock;

    bool m_isRunning = false;
};
