#pragma once

#include "game/Constants.hpp"

#include <SFML/System/Vector2.hpp>

#include <cstdint>
#include <limits>
#include <memory>

namespace cn::game
{

class Card;
using CardPtr = std::shared_ptr<Card>;

class Deck;
using DeckPtr = std::shared_ptr<Deck>;

class Discard;
using DiscardPtr = std::shared_ptr<Discard>;

class Player;
using PlayerPtr = std::shared_ptr<Player>;

class Table;
using TablePtr = std::shared_ptr<Table>;

struct PlayerSpawnPoint
{
    sf::Vector2f pos{};
    float angleDeg = 0;
};

using PlayerSlotId = uint8_t;
constexpr PlayerSlotId PlayerSlotIdInvalid = std::numeric_limits<PlayerSlotId>::max();

static_assert(std::numeric_limits<PlayerSlotId>::max() > MaxNumberOfPlayerSlots, "Adjust the max number of player slots");

using PlayerId = uint8_t;
constexpr PlayerId PlayerIdInvalid = std::numeric_limits<PlayerId>::max();

} // namespace cn::game
