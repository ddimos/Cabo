#pragma once

#include <cstdint>

namespace cn::shared::game
{

enum class StepId : uint8_t
{
    DecideCard,
    DiscardCard,
    DrawCard,
    FinishTurn,
    Idle,
    MatchCard,
    SeeAllCards,
    SeeOwnCard,
    SeeSomeonesCard,
    SwapCardBlindly,
    SwapCardOpenly,
    TakeCard
};

} // namespace cn::shared::game