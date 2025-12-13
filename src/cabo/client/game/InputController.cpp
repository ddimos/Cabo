#include "client/game/InputController.hpp"
#include "client/player/Manager.hpp"
#include "core/event/Dispatcher.hpp"
#include "shared/net/Manager.hpp"
#include "shared/events/InputEvents.hpp"
#include "shared/events/NetworkEvents.hpp"
#include "shared/game/Constants.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::game
{

InputController::InputController(core::Context& _context, Callback _moveCallback, Callback _releaseCallback, Callback _flipCallback)
    : m_contextRef(_context), m_moveCallback(_moveCallback), m_releaseCallback(_releaseCallback), m_flipCallback(_flipCallback)
{
    m_listenerId = core::event::getNewListenerId();
}

void InputController::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    auto& netManRef = m_contextRef.get<net::Manager>();
    auto& playerManRef = m_contextRef.get<player::Manager>();
    auto& windowRef = m_contextRef.get<sf::RenderWindow>();
    auto& clockRef = m_contextRef.get<sf::Clock>();
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::MouseButtonPressedEvent>(m_listenerId,
            [&netManRef, &playerManRef, &windowRef](const events::MouseButtonPressedEvent& _event){
                if (_event.mouseButton.button != sf::Mouse::Button::Left)
                    return;
                events::RemotePlayerInputNetEvent event(
                    playerManRef.getLocalPlayerId(), shared::game::PlayerInputType::Grab,
                    windowRef.mapPixelToCoords(sf::Vector2i(_event.mouseButton.x, _event.mouseButton.y)));
                netManRef.send(event);
            }
        );
        _dispatcher.registerEvent<events::MouseButtonReleasedEvent>(m_listenerId,
            [this, &netManRef, &playerManRef, &windowRef](const events::MouseButtonReleasedEvent& _event){
                if (_event.mouseButton.button != sf::Mouse::Button::Left)
                    return;
                sf::Vector2f pos = windowRef.mapPixelToCoords(sf::Vector2i(_event.mouseButton.x, _event.mouseButton.y));
                m_releaseCallback(pos);
                events::RemotePlayerInputNetEvent event(playerManRef.getLocalPlayerId(), shared::game::PlayerInputType::Release, pos);
                netManRef.send(event);

                events::RemotePlayerInputNetEvent clickEvent(playerManRef.getLocalPlayerId(), shared::game::PlayerInputType::Click, pos);
                netManRef.send(clickEvent);
            }
        );
        _dispatcher.registerEvent<events::KeyReleasedEvent>(m_listenerId,
            [this, &netManRef, &playerManRef, &windowRef](const events::KeyReleasedEvent& _event){
                if (_event.key.code != sf::Keyboard::Space)
                    return;
                auto mousePos = windowRef.mapPixelToCoords(sf::Mouse::getPosition(windowRef));
                m_flipCallback(mousePos);
                events::RemotePlayerInputNetEvent event(
                    playerManRef.getLocalPlayerId(), shared::game::PlayerInputType::Flip,
                    mousePos);
                netManRef.send(event);
            }
        );
        _dispatcher.registerEvent<events::MouseMovedEvent>(m_listenerId,
            [this, &netManRef, &playerManRef, &windowRef, &clockRef](const events::MouseMovedEvent& _event){
                sf::Vector2f pos = windowRef.mapPixelToCoords(sf::Vector2i(_event.mouseMove.x, _event.mouseMove.y));
                m_moveCallback(pos);

                auto currentTime = clockRef.getElapsedTime();
                if (currentTime - m_lastUpdateTime < shared::game::MoveUpdateDuration)
                    return;
                m_lastUpdateTime = currentTime;

                events::RemotePlayerInputNetEvent event(playerManRef.getLocalPlayerId(), shared::game::PlayerInputType::Move, pos);
                netManRef.send(event, false);
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::MouseButtonPressedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::MouseButtonReleasedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::KeyReleasedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::MouseMovedEvent>(m_listenerId);
    }
}

} // namespace cn::client::game
