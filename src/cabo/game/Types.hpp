#pragma once

#include <memory>

namespace cn::game
{

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

} // namespace cn::game
