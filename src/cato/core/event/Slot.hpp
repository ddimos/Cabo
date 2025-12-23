#pragma once

#include "core/event/Types.hpp"

#include <functional>
#include <memory>

namespace cn::core::event
{

struct SlotBase
{
    virtual ~SlotBase() = default;
    virtual void call(const Event& _event) = 0;
};
using SlotBasePtr = std::unique_ptr<SlotBase>;

template<typename TEvent>
struct Slot : public SlotBase
{
    using Callback = std::function<void(const TEvent&)>;
    Slot(Callback _callback)
        : m_callback(_callback)
    {}

    void call(const Event& _event) override { callSpecific(static_cast<const TEvent&>(_event)); }
    void callSpecific(const TEvent& _event) { m_callback(_event); }

private:
    Callback m_callback;
};

} // namespace cn::core::event
