#pragma once

#include "core/event/Types.hpp"
#include "core/state/State.hpp"

namespace cn::client::states
{

class FinishState final : public core::state::State
{
public:
    FinishState(core::state::Manager& _stateManagerRef);

private:
    core::state::Return onUpdate(sf::Time _dt) override;

    core::event::ListenerId m_listenerId{};
};

} // namespace cn::client::states
