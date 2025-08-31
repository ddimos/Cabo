#include "client/player/Manager.hpp"
#include "client/ResourceIds.hpp"

#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"
#include "shared/player/Types.hpp"

#include "core/Assert.hpp"
#include "core/Log.hpp"

#include <algorithm>

namespace cn::client::player
{

Manager::Manager(core::Context& _context)
    : m_contextRef(_context)
{
    m_listenerId = core::event::getNewListenerId();
}

Manager::~Manager() = default;

void Manager::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::PlayerJoinAcceptNetEvent>(m_listenerId,
            [this](const events::PlayerJoinAcceptNetEvent& _event){
                CN_LOG_I_FRM("Player id: {}", _event.m_playerId.value());
                m_localPlayerId = _event.m_playerId;

                auto& netManRef = m_contextRef.get<net::Manager>();
                auto& save = m_contextRef.get<SaveHolder>().get(SaveIds::PlayerName);

                events::PlayerUpdateNetEvent event({ Player{ .name = save.getValue() } });
                netManRef.send(event);

                m_players.emplace_back(Player{ .name = save.getValue(), .id = m_localPlayerId});
            }
        );
        _dispatcher.registerEvent<events::PlayerUpdateNetEvent>(m_listenerId,
            [this](const events::PlayerUpdateNetEvent& _event){
                CN_LOG_I_FRM("Player info.. players count: {}", _event.m_players.size());
                m_players.clear();
                m_players = _event.m_players;
            }
        );        
    }
    else
    {
        _dispatcher.unregisterEvent<events::PlayerJoinAcceptNetEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::PlayerUpdateNetEvent>(m_listenerId);
    }
}

const Player* Manager::getPlayer(PlayerId _playerId) const
{
    for (const auto& player : m_players)
    {
        if (player.id == _playerId)
            return &player;
    }
    CN_ASSERT_FRM(false, "Couldn't find the player {}", _playerId.value());
    return nullptr;
}

size_t Manager::getIndexOfLocalPlayer() const
{
    CN_ASSERT(m_localPlayerId.isValid());
    
    for (size_t i = 0; i < m_players.size(); ++i)
    {
        if (m_players[i].id == m_localPlayerId)
            return i;
    }
    CN_ASSERT_FRM(false, "No player with a local id: {}", m_localPlayerId.value());
    return 0;
}
} // namespace cn::client::player
