#pragma once

#include "shared/game/object/Deck.hpp"

namespace cn::client::game
{

class Deck final : public shared::game::object::Deck
{
public:
    Deck(shared::game::object::Id _id, unsigned _seed)
        : shared::game::object::Deck(_id, _seed) {}
private:
};

} // namespace cn::client::game
