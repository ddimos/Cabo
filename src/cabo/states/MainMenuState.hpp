#pragma once

#include "state/State.hpp"

namespace cn::states
{

class MainMenuState final : public state::State
{
public:
    MainMenuState(state::StateManager& _stateManagerRef);
};

} // namespace cn::states
