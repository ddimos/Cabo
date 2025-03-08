#pragma once

#include <utility>

namespace cn::shared::game
{

class Card
{
public:
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
        King,
        // TODO Joker
    };

    enum class Suit
    {
        Clubs,      // (♣)
        Hearts,     // (♥)
        Spades,     // (♠)
        Diamonds    // (♦)
    }; 

    enum class Ability
    {
        None,
        Peek,
        Spy,
        SwapBlindly,
        SwapOpenly
    };

    enum class State
    {
        InDeck,
        OnTable,
        OnHand,
        InDiscard
    };

    static std::pair<Rank, Suit> getCardFromIndex(unsigned _index);
    static bool hasAbility(Card _card);
    static Ability getAbility(Card _card);

    Card() = default;
    Card(Rank _rank, Suit _suit);

    Rank getRank() const { return m_rank; }
    Suit getSuit() const { return m_suit; }
    
    // TODO to figure out if I need these methods, State, inheritance from Object
    void deal(/*player id*/ /*position*/);
    void discard();

private:
    Rank m_rank = Rank::Ace;
    Suit m_suit = Suit::Clubs;

    State m_state = State::InDeck;
};

} // namespace cn::shared::game
