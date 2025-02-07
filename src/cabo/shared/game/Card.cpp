#include "shared/game/Card.hpp"
#include "core/Assert.hpp"
#include <cmath>

namespace cn::shared::game
{

std::pair<Card::Rank, Card::Suit> Card::getCardFromIndex(unsigned _index)
{
/*
            Clubs       Hearts      Spades     Diamonds
    Ace,      0           13          26          39
    _2,       1           14          27          40
    _3,       2           15          28          41
    _4,       3           16          29          42
    _5,       4           17          30          43
    _6,       5           18          31          44
    _7,       6           19          32          45
    _8,       7           20          33          46
    _9,       8           21          34          47
    _10,      9           22          35          48
    Jack,    10           23          36          49
    Queen,   11           24          37          50
    King,    12           25          38          51
*/
    constexpr unsigned NumOfRanks = 13;
    constexpr unsigned NumOfSuits = 4;
    static_assert(NumOfRanks - 1 == static_cast<unsigned>(Rank::King));
    static_assert(NumOfSuits - 1 == static_cast<unsigned>(Suit::Diamonds));

    CN_ASSERT(NumOfRanks * NumOfSuits > _index);

    unsigned rank = _index % NumOfRanks;
    unsigned suit = static_cast<unsigned>(std::floor(_index / NumOfRanks));
    CN_ASSERT(NumOfSuits > suit);

    return std::make_pair(static_cast<Rank>(rank), static_cast<Suit>(suit));
}

bool Card::hasAbility(Card& _card)
{
    return _card.m_rank == Rank::_7
        || _card.m_rank == Rank::_8
        || _card.m_rank == Rank::_9
        || _card.m_rank == Rank::_10
        || _card.m_rank == Rank::Jack
        || _card.m_rank == Rank::Queen;
}

Card::Card(Rank _rank, Suit _suit)
    : m_rank(_rank), m_suit(_suit)
{
}

void Card::deal(/*player id*/ /*position*/)
{
}

void Card::discard()
{
}

} // namespace cn::shared::game
