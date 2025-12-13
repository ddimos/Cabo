#pragma once

#include "shared/game/object/Object.hpp"
#include "shared/game/component/ClickCountable.hpp"

namespace cn::shared::game::object
{

class CountableButton : public Object
{
public:
    CountableButton(Id _id, unsigned _numberOfPlayersToClick, 
        component::ClickCountable::OnPlayerClickedFunc _playerClickedFunc,
        component::ClickCountable::OnResetFunc _onResetFunc);

    component::ClickCountable& getClickCountableComponent() { return m_clickCountableComponent; }

private:
    component::ClickCountable m_clickCountableComponent;
};

} // namespace cn::shared::game::object
