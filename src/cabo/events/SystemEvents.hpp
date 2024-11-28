#pragma once
#include "core/event/Event.hpp"
#include "events/EventIds.hpp"

#include <SFML/Window/Event.hpp>

namespace cn::events
{

class KeyReleasedEvent final : public core::event::Event
{
public:
    CN_EVENT(Id::OnKeyReleased)

    KeyReleasedEvent(sf::Event::KeyEvent _event)
        : core::event::Event(), key(_event) 
    {}

    sf::Event::KeyEvent key;
};

class MouseButtonReleasedEvent final : public core::event::Event
{
public:
    CN_EVENT(Id::OnMouseButtonReleased)

    MouseButtonReleasedEvent(sf::Event::MouseButtonEvent _event)
        : core::event::Event(), mouseButton(_event) 
    {}

    sf::Event::MouseButtonEvent mouseButton;
};

class MouseMovedEvent final : public core::event::Event
{
public:
    CN_EVENT(Id::OnMouseMoved)

    MouseMovedEvent(sf::Event::MouseMoveEvent _event)
        : core::event::Event(), mouseMove(_event) 
    {}

    sf::Event::MouseMoveEvent mouseMove;
};

} // namespace cn::events
