#pragma once

#include "core/event/Types.hpp"
#include "core/state/State.hpp"

#include <nsf/Types.hpp>

#include <unordered_set>

namespace cn::server::states
{

class LobbyState final : public core::state::State
{
public:
    LobbyState(core::state::Manager& _stateManagerRef);

private:
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;

    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
    std::unordered_set<nsf::PeerID> m_players;
};

} // namespace cn::server::states
