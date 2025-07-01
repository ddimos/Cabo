#pragma once

#include "core/event/Types.hpp"
#include "core/Context.hpp"

#include <SFML/System/Vector2.hpp>
#include <functional>

namespace cn::client::game
{

class InputController final
{
public:
    using MoveCallback = std::function<void(sf::Vector2f)>;
    InputController(core::Context& _context, MoveCallback _moveCallback);

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

private:
    core::Context& m_contextRef;
    core::event::ListenerId m_listenerId{};
    MoveCallback m_moveCallback{};
};

} // namespace cn::client::game
