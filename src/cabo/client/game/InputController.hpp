#pragma once

#include "core/event/Types.hpp"
#include "core/Context.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <functional>

namespace cn::client::game
{

class InputController final
{
public:
    using Callback = std::function<void(sf::Vector2f)>;
    InputController(core::Context& _context, Callback _moveCallback, Callback _releaseCallback, Callback _flipCallback);

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

private:
    core::Context& m_contextRef;
    core::event::ListenerId m_listenerId{};
    sf::Time m_lastUpdateTime{};
    Callback m_moveCallback{};
    Callback m_releaseCallback{};
    Callback m_flipCallback{};
};

} // namespace cn::client::game
