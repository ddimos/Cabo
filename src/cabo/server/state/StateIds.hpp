#pragma once
#include "core/state/StateTypes.hpp"

namespace cn::server::states
{

namespace id
{
    enum : core::state::StateId
    {
        Finish,
        Game,
        Loading,
        Lobby
    };
} // namespace id

} // namespace cn::server::states
