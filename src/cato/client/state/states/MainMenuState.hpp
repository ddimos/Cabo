#pragma once

#include "core/state/State.hpp"

namespace cn::client::states
{

class MainMenuState final : public core::state::State
{
public:
    MainMenuState(core::state::Manager& _stateManagerRef);

private:
    void onActivate() override;
    core::state::Return onUpdate(sf::Time _dt) override;

    bool m_goToEntering = true;
};

} // namespace cn::client::states
