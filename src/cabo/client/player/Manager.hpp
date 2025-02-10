#pragma once

#include "core/event/Dispatcher.hpp"

#include "shared/player/Types.hpp"

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
    PlayerId getLocalPlayerId() const { return m_localPlayerId; }
    
private:
    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
    std::vector<Player> m_players;
    PlayerId m_localPlayerId = PlayerIdInvalid;
};

} // namespace cn::client::player
