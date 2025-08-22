#include "shared/game/CardValue.hpp"

#include "core/Assert.hpp"
#include <cmath>

namespace
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
static_assert(NumOfRanks - 1 == static_cast<unsigned>(cn::shared::game::Rank::King));
static_assert(NumOfSuits - 1 == static_cast<unsigned>(cn::shared::game::Suit::Diamonds));

} // namespace

namespace cn::shared::game
{

object::Card::Value getValueFromCard(Rank _rank, Suit _suit)
{
    return object::Card::Value(static_cast<uint8_t>(_rank) + NumOfRanks * static_cast<uint8_t>(_suit));
}

std::pair<Rank, Suit> getCardFromValue(object::Card::Value _value)
{
    CN_ASSERT(NumOfRanks * NumOfSuits > _value.value());

    unsigned rank = _value.value() % NumOfRanks;
    unsigned suit = static_cast<unsigned>(std::floor(_value.value() / NumOfRanks));
    CN_ASSERT(NumOfSuits > suit);

    return std::make_pair(static_cast<Rank>(rank), static_cast<Suit>(suit));
}


} // namespace cn::shared::game
