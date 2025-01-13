#pragma once

#include "core/state/State.hpp"

namespace cn::server::states
{

class LoadingState final : public core::state::State
{
public:
    LoadingState(core::state::Manager& _stateManagerRef);
    core::state::Return onUpdate(sf::Time _dt) override;
};

} // namespace cn::server::states
