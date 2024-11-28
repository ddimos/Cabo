#pragma once

#include "core/state/State.hpp"

namespace cn::states
{

class MainMenuState final : public core::state::State
{
public:
    MainMenuState(core::state::Manager& _stateManagerRef);
};
} // namespace cn::states
