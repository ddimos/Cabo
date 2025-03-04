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
    SeeOwnCard, // ? SeeCardInSlot
    SeeSomeonesCard,
    SwapCardBlindly,
    SwapCardOpenly,
    TakeCard
};

} // namespace cn::shared::game