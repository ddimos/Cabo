#pragma once

#include "shared/game/object/Object.hpp"
#include "shared/game/component/ClickCountable.hpp"
#include "shared/game/Types.hpp"

namespace cn::shared::game::object
{

class CountableButton : public Object
{
public:
    CountableButton(Id _id, TableButtonType _type, unsigned _numberOfPlayersToClick, 
        component::ClickCountable::OnPlayerClickedFunc _playerClickedFunc,
        component::ClickCountable::OnResetFunc _onResetFunc);

    component::ClickCountable& getClickCountableComponent() { return m_clickCountableComponent; }
    TableButtonType getType() const { return m_type; }

private:
    component::ClickCountable m_clickCountableComponent;
    TableButtonType m_type = TableButtonType::Deal;
};

} // namespace cn::shared::game::object
