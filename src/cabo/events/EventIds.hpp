#pragma once
#include "core/event/Types.hpp"

namespace cn::events
{

// TODO rename
enum Id : core::event::EventId
{
    KeyReleased,
    MouseButtonReleased,
    MouseMoved,

    PlayerClickDeck,
    LocalPlayerClickSlot
};

} // namespace cn::events
