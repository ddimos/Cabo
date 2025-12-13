#include "shared/game/object/CountableButton.hpp"

namespace cn::shared::game::object
{

CountableButton::CountableButton(Id _id, unsigned _numberOfPlayersToClick, 
        component::ClickCountable::OnPlayerClickedFunc _playerClickedFunc,
        component::ClickCountable::OnResetFunc _onResetFunc)
    : Object(_id)
    , m_clickCountableComponent(*this, _numberOfPlayersToClick, _playerClickedFunc, _onResetFunc)
{
}

} // namespace cn::shared::game::object
