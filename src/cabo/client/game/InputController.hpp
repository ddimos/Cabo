#pragma once

#include "core/event/Types.hpp"
#include "core/Context.hpp"

namespace cn::client::game
{

class InputController final
{
public:
    InputController(core::Context& _context);

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

private:
    core::Context& m_contextRef;
    core::event::ListenerId m_listenerId{};
};

} // namespace cn::client::game
