#pragma once

#include "core/event/Types.hpp"
#include "core/Context.hpp"

#include "shared/events/NetworkEvents.hpp"

#include <functional>
#include <map>

namespace cn::server::game
{

class InputController final
{
public:
    using ProcessInputCallback = std::function<void(const events::RemotePlayerInputNetEvent&)>;
    InputController(core::Context& _context, ProcessInputCallback _processInput);

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);
    void update();

private:
    core::Context& m_contextRef;
    ProcessInputCallback m_processInput{};
    core::event::ListenerId m_listenerId{};

    std::map<int32_t, events::RemotePlayerInputNetEvent> m_inputBuffer;
};

} // namespace cn::server::game
