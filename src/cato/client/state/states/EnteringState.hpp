#pragma once

#include "core/state/State.hpp"
#include <string>

namespace cn::client::menu::item
{
    class Button;
    class Input;
}

namespace cn::client::states
{

class EnteringState final : public core::state::State
{
public:
    EnteringState(core::state::Manager& _stateManagerRef);

private:
    core::state::Return onUpdate(sf::Time _dt) override;
    
    menu::item::Button* m_buttonPtr = nullptr;
    menu::item::Input* m_inputNamePtr = nullptr;
    menu::item::Input* m_inputServerPtr = nullptr;
};

} // namespace cn::client::states
