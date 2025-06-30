#pragma once

#include "shared/game/Constants.hpp"
#include "shared/player/Types.hpp"

#include "core/Identifier.hpp"

#include <variant>

#include <SFML/System/Vector2.hpp>

namespace cn::shared::game
{

struct ParticipantSlot;
using ParticipantSlotId = core::Identifier<ParticipantSlot, uint8_t>;

static_assert(std::numeric_limits<ParticipantSlotId::Type>::max() > MaxNumberOfParticipantSlots, "Adjust the max number of player slots");

struct ParticipantSlot
{
    ParticipantSlotId id{};
    bool wasRemoved = false;
};

enum class BoardState
{
    Start,
    Peek,
    Game,
    Cabo,
    Finish
};

enum class ActionType
{
    Ability,
    Discard,
    Match,
    Take
};

enum class PileType
{
    Deck,
    Discard
};

// TODO remove
enum class InputType
{
    Action,
    Cabo,
    ClickPile,
    ClickSlot,
    Finish,
    SwapDecision
};

struct ClickSlotInputData
{
    ParticipantSlotId slotId{};
    PlayerId playerId{};
};

using InputDataVariant = std::variant<ActionType, ClickSlotInputData, PileType, bool, std::monostate>;

enum class PlayerInputType
{
    PressMouse,
    ReleaseMouse
};

using PlayerInputDataVariant = std::variant<sf::Vector2f, std::monostate>;

} // namespace cn::shared::game
