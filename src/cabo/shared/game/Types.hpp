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

enum class BoardState
{
    Start,
    Peek,
    Game,
    Cabo,
    Finish
};

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
