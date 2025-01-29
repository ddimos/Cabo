#include "server/player/Manager.hpp"

#include "shared/events/ConnectionEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "core/Assert.hpp"
#include "core/Log.hpp"

#include <algorithm>

namespace cn::server::player
{

Manager::Manager()
{
    m_listenerId = core::event::getNewListenerId();
}

Manager::~Manager() = default;

void Manager::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{

    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::PeerConnectedEvent>(m_listenerId,
            [this](const events::PeerConnectedEvent& _event){
                (void)_event;
                CN_LOG_FRM("Player joined.. id: {}", _event.m_peerId);
                m_players.emplace_back(Player{ .id = _event.m_peerId });
            }
        );

        _dispatcher.registerEvent<events::PeerDisconnectedEvent>(m_listenerId,
            [this](const events::PeerDisconnectedEvent& _event){
                CN_LOG_FRM("Player left.. id: {}", _event.m_peerId);
                m_players.erase( 
                    std::remove_if(m_players.begin(), m_players.end(),
                    [_event](const Player& _player){
                        return _player.id == _event.m_peerId;
                    }),
                    m_players.end()
                );
            }
        );

        _dispatcher.registerEvent<events::PlayerInfoUpdateEvent>(m_listenerId,
            [this](const events::PlayerInfoUpdateEvent& _event){
                auto it = std::find_if(m_players.begin(), m_players.end(),
                    [_event](const Player& _player){
                        return _player.id == _event.m_peerId;
                    }
                );
                CN_ASSERT(m_players.end() != it);
                CN_ASSERT(!m_players.empty());
                it->name = _event.m_players.front().name;
                CN_LOG_FRM("Player info.. if: {}, name: {}", it->id, it->name);
            }
        );        
    }
    else
    {
        _dispatcher.unregisterEvent<events::PeerConnectedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::PeerDisconnectedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::PlayerInfoUpdateEvent>(m_listenerId);
    }
}

} // namespace cn::server::player
