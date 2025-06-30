#include "client/game/InputController.hpp"
#include "client/player/Manager.hpp"
#include "core/event/Dispatcher.hpp"
#include "shared/net/Manager.hpp"
#include "shared/events/InputEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::game
{

InputController::InputController(core::Context& _context)
    : m_contextRef(_context)
{
    m_listenerId = core::event::getNewListenerId();
}

void InputController::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    auto& netManRef = m_contextRef.get<net::Manager>();
    auto& playerManRef = m_contextRef.get<player::Manager>();
    auto& windowRef = m_contextRef.get<sf::RenderWindow>();
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::MouseButtonPressedEvent>(m_listenerId,
            [&netManRef, &playerManRef, &windowRef](const events::MouseButtonPressedEvent& _event){
                if (_event.mouseButton.button != sf::Mouse::Button::Left)
                    return;
                events::RemotePlayerInputNetEvent2 event(
                    playerManRef.getLocalPlayerId(), shared::game::PlayerInputType::PressMouse,
                    windowRef.mapPixelToCoords(sf::Vector2i(_event.mouseButton.x, _event.mouseButton.y)));
                netManRef.send(event);
            }
        );
        _dispatcher.registerEvent<events::MouseButtonReleasedEvent>(m_listenerId,
            [&netManRef, &playerManRef, &windowRef](const events::MouseButtonReleasedEvent& _event){
                if (_event.mouseButton.button != sf::Mouse::Button::Left)
                    return;
                events::RemotePlayerInputNetEvent2 event(
                    playerManRef.getLocalPlayerId(), shared::game::PlayerInputType::ReleaseMouse, 
                    windowRef.mapPixelToCoords(sf::Vector2i(_event.mouseButton.x, _event.mouseButton.y)));
                netManRef.send(event);
            }
        );
        _dispatcher.registerEvent<events::MouseMovedEvent>(m_listenerId,
            [this](const events::MouseMovedEvent& _event){
                // TODO to send an update every 100 ms

            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::MouseButtonPressedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::MouseButtonReleasedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::MouseMovedEvent>(m_listenerId);
    }
}

} // namespace cn::client::game
