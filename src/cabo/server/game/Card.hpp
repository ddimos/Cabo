#pragma once

#include "core/object/Object.hpp"
#include "shared/game/Card.hpp"

namespace cn::server::game
{
// Make the card class a game object for a better lifetime management

class Card final: public core::object::Object, public shared::game::Card 
{
public:
    Card(Rank _rank, Suit _suit)
        : shared::game::Card(_rank, _suit)
    { }
};

} // namespace cn::server::game
