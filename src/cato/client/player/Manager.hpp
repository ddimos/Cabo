#pragma once

#include "core/event/Dispatcher.hpp"
#include "core/Context.hpp"

#include "shared/player/Types.hpp"

#include <vector>

namespace cn::client::player
{

class Manager final
{
public:
    Manager(core::Context& _context);
    ~Manager();

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

    const std::vector<Player>& getPlayers() const { return m_players; }
    const Player* getPlayer(PlayerId _playerId) const;

    PlayerId getLocalPlayerId() const { return m_localPlayerId; }
    size_t getIndexOfLocalPlayer() const;
    
private:
    core::Context& m_contextRef;

    core::event::ListenerId m_listenerId{};
    std::vector<Player> m_players;
    PlayerId m_localPlayerId{};
};

} // namespace cn::client::player
