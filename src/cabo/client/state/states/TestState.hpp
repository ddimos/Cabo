#pragma once

#include "core/state/State.hpp"


namespace cn::client::states
{

class TestState final : public core::state::State
{
public:
    TestState(core::state::Manager& _stateManagerRef);

private:
    void onDraw() override;

};

} // namespace cn::client::states
