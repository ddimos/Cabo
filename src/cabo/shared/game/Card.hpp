#pragma once

#include "shared/board/Card.hpp"
#include "shared/game/Object.hpp"

namespace cn::shared::game
{

class Card : public shared::game::Object
{
public:
    Card(shared::board::Card& _boardCard) : m_boardCardRef(_boardCard) {}

    shared::board::Object& getBoardObject() const { return m_boardCardRef; }
    shared::board::Card& getBoardCard() const { return m_boardCardRef; }

protected:
    shared::board::Card& m_boardCardRef;
};

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

board::Card::Value getValueFromCard(Rank, Suit);
std::pair<Rank, Suit> getCardFromValue(board::Card::Value _value);

} // namespace cn::shared::game
