#pragma once

#include "core/Identifier.hpp"

#include <memory>

namespace cn::core::event
{

class Event;
using EventId = core::Identifier<Event, uint8_t>;

class Listener;
using ListenerId = core::Identifier<Listener, uint16_t>;

using EventPtr = std::unique_ptr<Event>;

class Dispatcher;

} // namespace cn::core::event

template <>
struct std::hash<cn::core::event::EventId>
{
    std::size_t operator()(const cn::core::event::EventId& _id) const
    {
        return std::hash<cn::core::event::EventId::Type>()(_id.value());
    }
};

template <>
struct std::hash<cn::core::event::ListenerId>
{
    std::size_t operator()(const cn::core::event::ListenerId& _id) const
    {
        return std::hash<cn::core::event::ListenerId::Type>()(_id.value());
    }
};
