#pragma once
#include "core/state/StateTypes.hpp"

namespace cn::client::states
{

namespace id
{
    enum : core::state::StateId
    {
        Finish,
        Game,
        Joining,
        Lobby,
        MainMenu,
        EnterName,
        Title
    };
} // namespace id

} // namespace cn::client::states
