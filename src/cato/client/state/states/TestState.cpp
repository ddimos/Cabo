#include "client/state/states/TestState.hpp"

namespace cn::client::states
{

TestState::TestState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
}

void TestState::onDraw()
{
}

} // namespace cn::client::states
