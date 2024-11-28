#pragma once
#include "core/event/Types.hpp"

namespace cn::events
{

enum Id : core::event::EventId
{
    OnKeyReleased,
    OnMouseButtonReleased,
    OnMouseMoved
};

} // namespace cn::events
