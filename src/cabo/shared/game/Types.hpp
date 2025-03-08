#pragma once

#include "shared/game/Constants.hpp"
#include "shared/player/Types.hpp"

#include <cstdint>
#include <limits>
#include <memory>
#include <variant>

namespace cn::shared::game
{

using ParticipantSlotId = uint8_t;
constexpr ParticipantSlotId ParticipantSlotIdInvalid = std::numeric_limits<ParticipantSlotId>::max();

static_assert(std::numeric_limits<ParticipantSlotId>::max() > MaxNumberOfParticipantSlots, "Adjust the max number of player slots");

struct ParticipantSlot
{
    ParticipantSlotId id = ParticipantSlotIdInvalid;
    bool wasRemoved = false;
};

enum class BoardState
{
    Start,
    Peek,
    Game,
    Cabo
};

enum class DecideButton
{
    Action,
    Discard,
    Match,
    Take
};

enum class PileType
{
    Deck,
    Discard
};

enum class InputType
{
    Cabo,
    ClickPile,
    ClickSlot,
    DecideButton,
    Finish
};

struct ClickSlotInputData
{
    ParticipantSlotId slotId = ParticipantSlotIdInvalid;
    PlayerId playerId = PlayerIdInvalid;
};

using InputDataVariant = std::variant<DecideButton, ClickSlotInputData, PileType, std::monostate>;

} // namespace cn::shared::game
