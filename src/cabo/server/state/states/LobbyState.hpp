#pragma once

#include "core/event/Types.hpp"
#include "core/state/State.hpp"

namespace cn::server::states
{

class LobbyState final : public core::state::State
{
public:
    LobbyState(core::state::Manager& _stateManagerRef);

private:
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;

    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
};

} // namespace cn::server::states
