#pragma once
#include "core/event/Types.hpp"

namespace cn::core::event
{

#define CN_EVENT(_id)                                                               \
static core::event::EventId getIdStatic() { return core::event::EventId(_id); }     \
core::event::EventId getId() const override { return getIdStatic(); }

class Event
{
public:
    virtual ~Event() = default;
    virtual EventId getId() const = 0;
};

} // namespace cn::core::event
