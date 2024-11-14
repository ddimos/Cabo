#pragma once

namespace cn::game
{

class Card
{
public:
    enum class Rank
    {
        Ace,
        _1,
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
        King,
        Joker
    };

    enum class Suit
    {
        Clubs,      // (♣)
        Diamonds,   // (♦)
        Hearts,     // (♥)
        Spades      // (♠)
    }; 

    enum class State
    {
        InDeck,
        OnTable,
        OnHand,
        InDiscard
    };

    Card(Rank _rank, Suit _suit);

    void deal(/*player id*/ /*position*/);
    void discard();

private:
    Rank m_rank;
    Suit m_suit;

    State m_state = State::InDeck;
};

} // namespace cn::game
