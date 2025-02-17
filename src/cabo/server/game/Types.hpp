#pragma once

#include "server/game/Card.hpp"
#include "shared/game/StepId.hpp"
#include "shared/game/Types.hpp"

namespace cn::server::game
{

using shared::game::ParticipantSlotId;

struct ParticipantSlot : public shared::game::ParticipantSlot
{
    Card* card = nullptr;
};

using shared::game::BoardState;
using shared::game::StepId;

} // namespace cn::server::game
