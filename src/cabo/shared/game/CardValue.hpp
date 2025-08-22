#pragma once

#include "shared/game/object/Card.hpp"
#include "shared/game/Types.hpp"

namespace cn::shared::game
{

object::Card::Value getValueFromCard(Rank, Suit);
std::pair<Rank, Suit> getCardFromValue(object::Card::Value _value);

} // namespace cn::shared::game
