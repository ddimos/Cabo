#include "Game.hpp"

#include <SFML/Window/Event.hpp>

#include "Constants.hpp"

namespace cn
{

Game::Game()
    : m_window(sf::VideoMode(WINDOWS_WIDTH, WINDOWS_HEIGHT), "CABOn", sf::Style::Close)
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
        while (accumulator >= FRAME_TIME)
        {
		    fixedUpdate(FRAME_TIME);
            accumulator -= FRAME_TIME;
        }

        m_window.clear(sf::Color(2, 17, 34));
    
		draw();

        m_window.display();
    } 
}

void Game::init()
{
    m_fontHolder.load(FontIds::Main, "res/fonts/times_new_roman.ttf");
    // m_textureHolder.load(TextureIds::Background, "res/textures/Background.png");
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

void Game::draw()
{
}

} // namespace cn
