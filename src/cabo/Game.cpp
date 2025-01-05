#include "Game.hpp"

#include "core/event/Dispatcher.hpp"
#include "core/Log.hpp"
#include "core/Types.hpp"

#include "events/SystemEvents.hpp"

#include "states/StateIds.hpp"
#include "states/FinishState.hpp"
#include "states/GameState.hpp"
#include "states/MainMenuState.hpp"
#include "states/TitleState.hpp"

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
    , m_stateManager(core::Context(m_window, m_textureHolder, m_fontHolder, m_eventManager.getDispatcher()))
{
    m_window.setKeyRepeatEnabled(false);
    m_window.setFramerateLimit(60);
}

void Game::start()
{
    CN_LOG("Start CABOn");

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

        update(elapsed);
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
    m_textureHolder.load(TextureIds::Background, "res/textures/background.png");
    m_textureHolder.load(TextureIds::Cards, "res/textures/cards.png");
    m_textureHolder.load(TextureIds::MainMenuStartButton, "res/textures/create_menu_start_button.png");
    m_textureHolder.load(TextureIds::DecideButtons, "res/textures/decide_buttons.png");
    m_textureHolder.load(TextureIds::MainMenuJoinButton, "res/textures/join_menu_join_button.png");
    m_textureHolder.load(TextureIds::Table, "res/textures/table.png");

    m_stateManager.registerState<states::TitleState>(states::id::Title);
    m_stateManager.registerState<states::MainMenuState>(states::id::MainMenu);
    m_stateManager.registerState<states::GameState>(states::id::Game);
    m_stateManager.registerState<states::FinishState>(states::id::Finish);

    m_stateManager.pushState(states::id::Title);
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
        case sf::Event::KeyReleased:
            // m_eventManager.getDispatcher().send(std::make_unique<events::KeyReleasedEvent>(event.key));
            m_eventManager.getDispatcher().send<events::KeyReleasedEvent>(event.key);
            break;
        case sf::Event::MouseButtonReleased:
            m_eventManager.getDispatcher().send<events::MouseButtonReleasedEvent>(event.mouseButton);
            break;
        case sf::Event::MouseMoved:
            m_eventManager.getDispatcher().send<events::MouseMovedEvent>(event.mouseMove);
            break;
        default:
            break;
        }
    }
    m_eventManager.process();
}

void Game::update(sf::Time _dt)
{
    m_stateManager.update(_dt);
}

void Game::fixedUpdate(sf::Time _dt)
{
}

void Game::draw()
{
    m_stateManager.draw();
}

} // namespace cn
