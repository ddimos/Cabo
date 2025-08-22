#pragma once

#include "shared/game/Constants.hpp"
#include "shared/player/Types.hpp"

#include "shared/game/object/Card.hpp"
#include "shared/game/object/Object.hpp"

#include "core/Identifier.hpp"

#include <variant>

#include <SFML/System/Vector2.hpp>

namespace cn::shared::game
{

enum class Rank
{
    Ace,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _10,
    Jack,
    Queen,
    King
};

enum class Suit
{
    Clubs,      // (♣)
    Hearts,     // (♥)
    Spades,     // (♠)
    Diamonds    // (♦)
}; 

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
    Flip,
    Grab,
    Move,
    Release
};

using PlayerInputDataVariant = std::variant<sf::Vector2f, std::monostate>;

enum class ServerCommandType
{
    PlayerInteractsWithCard,
    PlayerMoves,
    ProvideCardValue
};

struct PlayerInteractsWithCardData
{
    enum class Type
    {
        Grabs,
        Releases,
        TurnsDown,
        TurnsUp
    };
    PlayerId playerId{};
    object::Id cardId{};
    sf::Vector2f pos{};
    Type type = Type::Grabs;
};

struct PlayerMovesData
{
    PlayerId playerId{};
    sf::Vector2f pos{};
};

struct ProvideCardValueData
{
    object::Id cardId{};
    object::Card::Value value{};
};

using ServerCommandDataVariant = std::variant<PlayerInteractsWithCardData, PlayerMovesData, ProvideCardValueData, std::monostate>;

} // namespace cn::shared::game
