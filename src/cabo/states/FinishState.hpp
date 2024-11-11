#pragma once

#include "state/State.hpp"

namespace cn::states
{

class FinishState : public state::State
{
public:
    FinishState(state::StateManager& _stateManagerRef);

    Return onHandleEvent(const sf::Event& _event) override;
};

} // namespace cn::states
