#pragma once

#include "core/state/State.hpp"

namespace cn::server::states
{

class LoadingState final : public core::state::State
{
public:
    LoadingState(core::state::Manager& _stateManagerRef);
};

} // namespace cn::server::states
