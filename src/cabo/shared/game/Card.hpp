#pragma once

#include "core/Identifier.hpp"

#include <utility>

namespace cn::shared::game
{

class Card;
using CardId = core::Identifier<Card, uint8_t>;

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

    static std::pair<Rank, Suit> getCardFromIndex(unsigned _index);
    static bool hasAbility(const Card& _card);
    static Ability getAbility(const Card& _card);

    Card() = default;
    Card(Rank _rank, Suit _suit);

    Rank getRank() const { return m_rank; }
    Suit getSuit() const { return m_suit; }

    void setId(CardId _id) { m_id = _id; }
    CardId getId() const { return m_id; }

    bool hasAbility() const;
    Ability getAbility() const;

    bool isCardValueValid() const { return m_isCardValueValid; }
    
protected:
    void set(Rank _rank, Suit _suit);

private:
    Rank m_rank = Rank::Ace;
    Suit m_suit = Suit::Clubs;
    CardId m_id{};

    bool m_isCardValueValid = false;
};

} // namespace cn::shared::game
