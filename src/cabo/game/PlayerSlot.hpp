#pragma once

#include "game/Types.hpp"
#include "client/menu/Types.hpp"

namespace cn::game
{

struct PlayerSlot
{
    PlayerSlotId id;
    client::menu::item::ButtonPtr m_button;
    CardPtr card;
};

} // namespace cn::game
