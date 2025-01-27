#pragma once

#include "core/event/Dispatcher.hpp"

#include "Types.hpp"

#include <vector>

namespace cn::client::player
{

class Manager final
{
public:
    Manager();
    ~Manager();

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

    const std::vector<Player>& getPlayers() const { return m_players; }

private:
    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
    std::vector<Player> m_players;
};


} // namespace cn::client::player
