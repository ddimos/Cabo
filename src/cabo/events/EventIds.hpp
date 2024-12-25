#pragma once
#include "core/event/Types.hpp"

namespace cn::events
{

// TODO rename
enum Id : core::event::EventId
{
    OnKeyReleased,
    OnMouseButtonReleased,
    OnMouseMoved,

    PlayerClickDeck,
    LocalPlayerClickSlot
};

} // namespace cn::events
