#pragma once

#include "core/state/State.hpp"

namespace cn::client::states
{

class MainMenuState final : public core::state::State
{
public:
    MainMenuState(core::state::Manager& _stateManagerRef);
};

} // namespace cn::client::states
