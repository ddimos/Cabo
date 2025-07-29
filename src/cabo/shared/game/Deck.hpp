#pragma once

#include "shared/board/Deck.hpp"
#include "shared/game/Object.hpp"

namespace cn::shared::game
{

class Deck : public Object
{
public:
    Deck(shared::board::Deck& _boardDeck)
        : Object(_boardDeck)
    {}

    board::Deck& getBoardDeck() const { return static_cast<board::Deck&>(getBoardObject()); }
};

} // namespace cn::shared::game
