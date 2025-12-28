#pragma once
#include "core/event/Event.hpp"
#include "shared/events/EventIds.hpp"

#include <SFML/Window/Event.hpp>

namespace cn::events
{

class KeyReleasedEvent final : public core::event::Event
{
public:
    CN_EVENT(id::KeyReleased)

    KeyReleasedEvent(sf::Event::KeyEvent _event)
        : core::event::Event(), key(_event) 
    {}

    sf::Event::KeyEvent key;
};

class MouseButtonPressedEvent final : public core::event::Event
{
public:
    CN_EVENT(id::MouseButtonPressed)

    MouseButtonPressedEvent(sf::Event::MouseButtonEvent _event)
        : core::event::Event(), mouseButton(_event) 
    {}

    sf::Event::MouseButtonEvent mouseButton;
};

class MouseButtonReleasedEvent final : public core::event::Event
{
public:
    CN_EVENT(id::MouseButtonReleased)

    MouseButtonReleasedEvent(sf::Event::MouseButtonEvent _event)
        : core::event::Event(), mouseButton(_event) 
    {}

    sf::Event::MouseButtonEvent mouseButton;
};

class MouseMovedEvent final : public core::event::Event
{
public:
    CN_EVENT(id::MouseMoved)

    MouseMovedEvent(sf::Event::MouseMoveEvent _event)
        : core::event::Event(), mouseMove(_event) 
    {}

    sf::Event::MouseMoveEvent mouseMove;
};

class TextEnteredEvent final : public core::event::Event
{
public:
    CN_EVENT(id::TextEntered)

    TextEnteredEvent(sf::Event::TextEvent _event)
        : core::event::Event(), text(_event) 
    {}

    sf::Event::TextEvent text;
};

class TextPastedEvent final : public core::event::Event
{
public:
    CN_EVENT(id::TextPasted)

    TextPastedEvent(const sf::String& _text)
        : core::event::Event(), text(_text) 
    {}

    sf::String text;
};

} // namespace cn::events
