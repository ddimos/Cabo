#pragma once

#include "core/state/State.hpp"

namespace cn::states
{

class FinishState final : public core::state::State
{
public:
    FinishState(core::state::Manager& _stateManagerRef);

    core::state::Return onHandleEvent(const sf::Event& _event) override;
};

} // namespace cn::states
