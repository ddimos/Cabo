#pragma once
#include "core/event/Event.hpp"
#include "events/EventIds.hpp"

// #include <nsf/Types.hpp>
#include <string>

namespace cn::events
{

class PlayerInfoUpdateEvent final : public core::event::Event
{
public:
    CN_EVENT(id::PlayerInfoUpdate)

    PlayerInfoUpdateEvent() = default;
    PlayerInfoUpdateEvent(const std::string& _name)
        : m_name(_name)
    {}

    std::string m_name;
};

} // namespace cn::events
