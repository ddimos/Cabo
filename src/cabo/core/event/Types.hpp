#pragma once
#include <cstdint>
#include <limits>
#include <memory>

namespace cn::core::event
{

using EventId = uint16_t;
constexpr EventId EventIdInvalid = std::numeric_limits<EventId>::max();

using ListenerId = uint16_t;
constexpr ListenerId ListenerIdInvalid = std::numeric_limits<ListenerId>::max();

class Event;
using EventPtr = std::unique_ptr<Event>;

} // namespace cn::core::event
