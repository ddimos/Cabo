#pragma once

#include "core/event/Types.hpp"
#include "state/State.hpp"

namespace cn::states
{

class FinishState final : public state::State
{
public:
    FinishState(state::StateManager& _stateManagerRef);

private:
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;
    
    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
};

} // namespace cn::states
