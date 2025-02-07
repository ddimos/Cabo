#pragma once

#include "shared/game/Constants.hpp"

#include <cstdint>
#include <limits>
#include <memory>

namespace cn::shared::game
{

using ParticipantSlotId = uint8_t;
constexpr ParticipantSlotId ParticipantSlotIdInvalid = std::numeric_limits<ParticipantSlotId>::max();

static_assert(std::numeric_limits<ParticipantSlotId>::max() > MaxNumberOfParticipantSlots, "Adjust the max number of player slots");

struct ParticipantSlot
{
    ParticipantSlotId id;
};

} // namespace cn::shared::game
