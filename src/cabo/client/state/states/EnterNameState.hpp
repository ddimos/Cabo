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

class EnterNameState final : public core::state::State
{
public:
    EnterNameState(core::state::Manager& _stateManagerRef);

private:
    core::state::Return onUpdate(sf::Time _dt) override;
    
    menu::item::Button* m_buttonPtr = nullptr;
    menu::item::Input* m_inputPtr = nullptr;
};

} // namespace cn::client::states
