#pragma once

#include <memory>

namespace cn::game
{

class Deck;
using DeckPtr = std::shared_ptr<Deck>;

class Table;
using TablePtr = std::shared_ptr<Table>;

class Player;
using PlayerPtr = std::shared_ptr<Player>;

} // namespace cn::game
