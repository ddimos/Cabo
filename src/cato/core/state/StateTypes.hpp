#pragma once

#include <memory>
#include <cstdint>

namespace cn::core::state
{

using StateId = uint8_t;
inline constexpr StateId StateIdInvalid = 255;

class State;
using StatePtr = std::unique_ptr<State>;

enum class Return
{
    Continue,
    Break
};

} // namespace cn::core::state
