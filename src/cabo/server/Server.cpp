#include "server/Server.hpp"

#include "ContextObjectIds.hpp"
#include "LaunchTarget.hpp"

#include "core/event/Dispatcher.hpp"
#include "core/Log.hpp"

#include "shared/events/InputEvents.hpp"

#include "server/state/States.hpp"

#include "Constants.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/System/Sleep.hpp>


namespace cn::server
{

Server::Server()
    : m_netManager(m_context, true), m_stateManager(m_context), m_playerManager(m_context)
{
    m_context.insert(m_netManager);
    m_context.insert(m_eventManager.getDispatcher());
    m_context.insert(m_playerManager);
    m_context.insert(m_systemClock);

    m_netManager.init();
}

void Server::start()
{
    CN_LOG("Start Server CABOn");

    m_isRunning = true;
    m_systemClock.restart();

    init();
    
    sf::Clock clock;
    sf::Clock throttlingClock;
    sf::Time accumulator = sf::Time::Zero;

    while (m_isRunning)
    {
        sf::Time elapsed = clock.restart();
        sf::Time elapsedLoop = throttlingClock.restart();
        accumulator += elapsed;

        m_netManager.updateReceive();

        handleEvents();

        update(elapsed);
        while (accumulator >= TimePerFrame)
        {
            fixedUpdate(TimePerFrame);
            accumulator -= TimePerFrame;
        }

        m_netManager.updateSend();

        elapsedLoop = throttlingClock.restart();
        sf::Time t = TimePerFrame - elapsedLoop;
        if (t.asMilliseconds() > 0)
            sf::sleep(t);
    }

    deinit();
}

void Server::init()
{
    m_stateManager.registerState<states::LoadingState>(states::id::Loading);
    m_stateManager.registerState<states::LobbyState>(states::id::Lobby);
    m_stateManager.registerState<states::GameState>(states::id::Game);
    m_stateManager.registerState<states::FinishState>(states::id::Finish);

    m_stateManager.pushState(states::id::Loading);

    m_playerManager.registerEvents(m_eventManager.getDispatcher(), true);
}

void Server::deinit()
{
    m_playerManager.registerEvents(m_eventManager.getDispatcher(), false);
}

void Server::handleEvents()
{
    // sf::Event event;
    // while (m_window.pollEvent(event))
    // {
    //     switch (event.type)
    //     {
    //     case sf::Event::Closed:
    //         m_isRunning = false;
    //         break;
    //     case sf::Event::KeyReleased:
    //         // m_eventManager.getDispatcher().send(std::make_unique<events::KeyReleasedEvent>(event.key));
    //         m_eventManager.getDispatcher().send<events::KeyReleasedEvent>(event.key);
    //         break;
    //     case sf::Event::MouseButtonReleased:
    //         m_eventManager.getDispatcher().send<events::MouseButtonReleasedEvent>(event.mouseButton);
    //         break;
    //     case sf::Event::MouseMoved:
    //         m_eventManager.getDispatcher().send<events::MouseMovedEvent>(event.mouseMove);
    //         break;
    //     default:
    //         break;
    //     }
    // }
    m_eventManager.process();
}

void Server::update(sf::Time _dt)
{
    m_stateManager.update(_dt);
}

void Server::fixedUpdate(sf::Time _dt)
{
    (void)_dt;
}

} // namespace cn::server