#pragma once

#include "game/Types.hpp"
#include "menu/Types.hpp"

namespace cn::game
{

struct PlayerSlot
{
    PlayerSlotId id;
    menu::item::ButtonPtr m_button;
    CardPtr card;
};

} // namespace cn::game
