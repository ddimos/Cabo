#include "Game.hpp"

#include "state/states/TitleState.hpp"

#include "Constants.hpp"

#include <SFML/Window/Event.hpp>

namespace
{
    const sf::Time TimePerFrame = sf::seconds(cn::FRAME_TIME_s);
}

namespace cn
{

Game::Game()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CABOn", sf::Style::Close)
    , m_stateManager(state::Context(m_window, m_textureHolder, m_fontHolder))
{
}

void Game::start()
{
    m_isRunning = true;
    m_systemClock.restart();
    
    init();
    
    sf::Clock clock;
    sf::Time accumulator = sf::Time::Zero;

    while (m_isRunning)
    {
        sf::Time elapsed = clock.restart();
        accumulator += elapsed;

        handleEvents();

        update();
        while (accumulator >= TimePerFrame)
        {
           fixedUpdate(TimePerFrame);
            accumulator -= TimePerFrame;
        }

        m_window.clear(sf::Color(2, 17, 34));

        draw();

        m_window.display();
    } 
}

void Game::init()
{
    m_fontHolder.load(FontIds::Main, "res/fonts/times_new_roman.ttf");

    m_stateManager.registerState<state::TitleState>(state::Name::Title);

    m_stateManager.pushState(state::Name::Title);
}

void Game::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        m_stateManager.handleEvent(event);

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

void Game::fixedUpdate(sf::Time _dt)
{
    m_stateManager.update(_dt);
}

void Game::draw()
{
    m_stateManager.draw();
}

} // namespace cn
