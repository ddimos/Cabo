#include "client/Client.hpp"

#include "LaunchTarget.hpp"

#include "core/event/Dispatcher.hpp"
#include "core/Log.hpp"
#include "core/Path.hpp"

#include "shared/events/InputEvents.hpp"

#include "client/state/States.hpp"

#include "Constants.hpp"

#include <SFML/Window/Event.hpp>

namespace
{

std::string getResourcesPath()
{
#if defined(__APPLE__)
    return std::string(cn::core::getExecutablePath().parent_path().parent_path().c_str()) + "/Resources/";
#else
    return "res/";
#endif
}

} // namespace

namespace cn::client
{

Client::Client()
    : m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CABOn", sf::Style::Close)
    , m_netManager(m_context, false), m_stateManager(m_context), m_playerManager(m_context)
{
    m_context.insert(m_window);
    m_context.insert(m_textureHolder);
    m_context.insert(m_fontHolder);
    m_context.insert(m_saveHolder);
    m_context.insert(m_netManager);
    m_context.insert(m_eventManager.getDispatcher());
    m_context.insert(m_playerManager);
    m_context.insert(m_systemClock);
    m_context.insert(m_seed);

    m_window.setKeyRepeatEnabled(false);
    m_window.setFramerateLimit(60);

    m_netManager.init();
}

void Client::start()
{
    CN_LOG("Start Client CABOn");

    m_isRunning = true;
    m_systemClock.restart();
    
    init();
    
    sf::Clock clock;
    sf::Time accumulator = sf::Time::Zero;

    while (m_isRunning)
    {
        sf::Time elapsed = clock.restart();
        accumulator += elapsed;

        m_netManager.updateReceive();
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

        m_netManager.updateSend();
    }

    deinit();
}

void Client::init()
{
    auto resourcePath = getResourcesPath();
    m_fontHolder.load(FontIds::Main,                        resourcePath + "fonts/times_new_roman.ttf");
    m_textureHolder.load(TextureIds::Background,            resourcePath + "textures/background.png");
    m_textureHolder.load(TextureIds::BackButton,            resourcePath + "textures/back_button.png");
    m_textureHolder.load(TextureIds::CaboButton,            resourcePath + "textures/cabo_button.png");
    m_textureHolder.load(TextureIds::Cards,                 resourcePath + "textures/cards.png");
    m_textureHolder.load(TextureIds::DecideButtons,         resourcePath + "textures/decide_buttons.png");
    m_textureHolder.load(TextureIds::Field,                 resourcePath + "textures/field.png");
    m_textureHolder.load(TextureIds::FinishButton,          resourcePath + "textures/finish_button.png");
    m_textureHolder.load(TextureIds::MainMenuJoinButton,    resourcePath + "textures/join_menu_join_button.png");
    m_textureHolder.load(TextureIds::MainMenuStartButton,   resourcePath + "textures/create_menu_start_button.png");
    m_textureHolder.load(TextureIds::Table,                 resourcePath + "textures/table.png");
    m_textureHolder.load(TextureIds::YesNoButtons,          resourcePath + "textures/yes_no_buttons.png");

    m_saveHolder.load(SaveIds::PlayerName,                  resourcePath + "save/player_name");
    m_saveHolder.load(SaveIds::ServerAddress,               resourcePath + "save/server_address");

// TODO move states
    m_stateManager.registerState<states::TestState>(states::id::Test);
    m_stateManager.registerState<states::TitleState>(states::id::Title);
    m_stateManager.registerState<states::EnterNameState>(states::id::EnterName);
    m_stateManager.registerState<states::MainMenuState>(states::id::MainMenu);
    m_stateManager.registerState<states::JoiningState>(states::id::Joining);
    m_stateManager.registerState<states::LobbyState>(states::id::Lobby);
    m_stateManager.registerState<states::GameState>(states::id::Game);
    m_stateManager.registerState<states::FinishState>(states::id::Finish);

    m_stateManager.pushState(states::id::EnterName);

    m_playerManager.registerEvents(m_eventManager.getDispatcher(), true);
}

void Client::deinit()
{
    m_playerManager.registerEvents(m_eventManager.getDispatcher(), false);
}

void Client::handleEvents()
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
        case sf::Event::TextEntered:
            m_eventManager.getDispatcher().send<events::TextEnteredEvent>(event.text);
            break;
        default:
            break;
        }
    }
    m_eventManager.process();
}

void Client::update(sf::Time _dt)
{
    m_stateManager.update(_dt);
}

void Client::fixedUpdate(sf::Time _dt)
{
    (void)_dt;
}

void Client::draw()
{
    m_stateManager.draw();
}

} // namespace cn::client
