#include "client/player/Manager.hpp"

#include "shared/events/NetworkEvents.hpp"

#include "core/Assert.hpp"
#include "core/Log.hpp"

#include <algorithm>

namespace cn::client::player
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
        _dispatcher.registerEvent<events::PlayerInfoUpdateEvent>(m_listenerId,
            [this](const events::PlayerInfoUpdateEvent& _event){
                CN_LOG_FRM("Player info.. players count: {}", _event.m_players.size());
                m_players.clear();
                m_players = _event.m_players;
            }
        );        
    }
    else
    {
        _dispatcher.unregisterEvent<events::PlayerInfoUpdateEvent>(m_listenerId);
    }
}

} // namespace cn::client::player
