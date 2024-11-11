#pragma once

#include <memory>
#include <cstdint>

namespace cn::state
{

using StateId = uint8_t;
inline constexpr StateId StateIdInvalid = 255;

class State;
using StatePtr = std::unique_ptr<State>;

} // namespace cn::state
