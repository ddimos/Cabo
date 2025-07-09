#pragma once

#include "shared/game/Deck.hpp"

namespace cn::client::game
{

class Deck final : public shared::game::Deck
{
public:
    Deck(shared::board::Deck& _boardDeck)
        : shared::game::Deck(_boardDeck) {}
private:
};

} // namespace cn::client::game
