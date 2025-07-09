#pragma once

#include "shared/board/Deck.hpp"
#include "shared/game/Object.hpp"

namespace cn::shared::game
{

class Deck : public shared::game::Object
{
public:
    Deck(shared::board::Deck& _boardDeck) : m_boardDeckRef(_boardDeck) {}

    shared::board::Object& getBoardObject() const { return m_boardDeckRef; }
    shared::board::Deck& getBoardDeck() const { return m_boardDeckRef; }

protected:
    shared::board::Deck& m_boardDeckRef;
};

} // namespace cn::shared::game
