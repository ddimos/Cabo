#include "server/player/Manager.hpp"

#include "shared/events/ConnectionEvents.hpp"
#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"

#include "core/Assert.hpp"
#include "core/Log.hpp"

#include <algorithm>

namespace cn::server::player
{

Manager::Manager(core::Context& _contextRef)
    : m_contextRef(_contextRef)
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
                auto& player = m_players.emplace_back(Player{ .id = _event.m_peerId });

                auto& netManRef = m_contextRef.get<net::Manager>();
                events::PlayerJoinAcceptEvent event(player.id);
                netManRef.send(event, _event.m_peerId);
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
                        return _player.id == _event.m_senderPeerId;
                    }
                );
                CN_ASSERT(m_players.end() != it);
                CN_ASSERT(!m_players.empty());
                it->name = _event.m_players.front().name;
                CN_LOG_FRM("Player info.. id: {}, name: {}", it->id, it->name);

                auto& netManRef = m_contextRef.get<net::Manager>();
                events::PlayerInfoUpdateEvent event(m_players);
                netManRef.send(event);
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
