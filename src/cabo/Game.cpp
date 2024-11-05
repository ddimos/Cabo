#include "Game.hpp"

#include <SFML/Window/Event.hpp>

constexpr unsigned UPDATES_PER_SEC = 60;
constexpr float DT = 1.f / UPDATES_PER_SEC;

Game::Game()
    : m_window(sf::VideoMode(640, 480), "CABOn", sf::Style::Close)
{
}

void Game::start()
{
    m_isRunning = true;
    m_systemClock.restart();
    
    init();
    
    sf::Clock clock;
    float accumulator = 0.f;

    while (m_isRunning)
    {
        sf::Time elapsed = clock.restart();
        accumulator += elapsed.asSeconds();

        handleEvents();

		update();
        while (accumulator >= DT)
        {
		    fixedUpdate(DT);
            accumulator -= DT;
        }

        m_window.clear(sf::Color(2, 17, 34));
    
		render();

        m_window.display();
    } 
}

void Game::init()
{

}

void Game::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            m_isRunning = false;
            break;
        case sf::Event::KeyPressed:
            // m_pressedKeysState[event.key.code] = true;
            break;
        default:
            break;
        }
    }
}

void Game::update()
{
}

void Game::fixedUpdate(float _dt)
{
}

void Game::render()
{
}
