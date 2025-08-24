#include "server/game/InputController.hpp"

#include "shared/net/Manager.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "core/event/Dispatcher.hpp"
#include "core/Log.hpp"

#include <SFML/Window/Event.hpp>

namespace cn::server::game
{

InputController::InputController(core::Context& _context, ProcessInputCallback _processInput)
    : m_contextRef(_context), m_processInput(_processInput)
{
    m_listenerId = core::event::getNewListenerId();
}

void InputController::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    auto& netManRef = m_contextRef.get<net::Manager>();
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::RemotePlayerInputNetEvent>(m_listenerId,
            [this](const events::RemotePlayerInputNetEvent& _event){
    //            CN_LOG_FRM("Input {} from {} received", (unsigned)_event.m_type, _event.m_playerId.value());
                m_inputBuffer.emplace(_event.m_sentTimeRttBased.asMilliseconds(), _event);
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::RemotePlayerInputNetEvent>(m_listenerId);
    }
}

void InputController::update()
{
    constexpr int32_t TimeInInputBufferMs = 100; // TODO configure this or add to constants 
    
    for (auto it = m_inputBuffer.cbegin(); it != m_inputBuffer.cend();)
    {
        auto& systemClockRef = m_contextRef.get<sf::Clock>();
        int32_t currentTime = systemClockRef.getElapsedTime().asMilliseconds();
        if (currentTime - m_inputBuffer.begin()->first >= TimeInInputBufferMs)
        {
            m_processInput(it->second);
            m_inputBuffer.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

} // namespace cn::server::game
