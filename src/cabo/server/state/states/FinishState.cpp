#include "server/state/states/FinishState.hpp"
#include "server/state/StateIds.hpp"

#include "core/event/Dispatcher.hpp"
#include "core/Log.hpp"

namespace cn::server::states
{

FinishState::FinishState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    // TODO Do I need this state?
    m_listenerId = core::event::getNewListenerId();

    CN_LOG("Finish state..");
}

core::state::Return FinishState::onUpdate(sf::Time _dt)
{
    pop();
    push(id::Lobby);

    return core::state::Return::Break;
}

} // namespace cn::server::states
