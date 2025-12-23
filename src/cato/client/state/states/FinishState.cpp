#include "client/state/states/FinishState.hpp"
#include "client/state/StateIds.hpp"

namespace cn::client::states
{

FinishState::FinishState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
}

core::state::Return FinishState::onUpdate(sf::Time _dt)
{
    pop();
    push(id::Lobby);

    return core::state::Return::Break;
}

} // namespace cn::client::states
