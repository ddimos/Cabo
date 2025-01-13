#include "server/state/states/FinishState.hpp"
#include "server/state/StateIds.hpp"

#include "core/event/Dispatcher.hpp"
#include "events/SystemEvents.hpp"

namespace cn::server::states
{

FinishState::FinishState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    m_listenerId = core::event::getNewListenerId();
}

void FinishState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
    }
    else
    {
    }
}

} // namespace cn::server::states
