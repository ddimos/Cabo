#include "server/state/states/LoadingState.hpp"
#include "server/state/StateIds.hpp"

#include "core/Log.hpp"

namespace cn::server::states
{

LoadingState::LoadingState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    CN_LOG_I("Loading state..");
}

core::state::Return LoadingState::onUpdate(sf::Time _dt)
{
    pop();
    push(id::Lobby);

    return core::state::Return::Break;
}

} // namespace cn::server::states
