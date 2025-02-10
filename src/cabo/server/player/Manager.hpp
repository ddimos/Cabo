#pragma once

#include "core/event/Dispatcher.hpp"

#include "shared/player/Types.hpp"

#include <vector>

namespace cn::core
{
class Context;
} // namespace cn::core

namespace cn::server::player
{

class Manager final
{
public:
    Manager(core::Context& _contextRef);
    ~Manager();

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

    const std::vector<Player>& getPlayers() const { return m_players; }

private:
    core::Context& m_contextRef;
    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
    std::vector<Player> m_players;
};

} // namespace cn::server::player
