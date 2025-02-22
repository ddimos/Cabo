#pragma once

#include "shared/game/StepId.hpp"
#include "shared/game/Types.hpp"

namespace cn::shared::game
{

inline StepId getFirstStepId(BoardState _boardState)
{
    StepId firstStep = StepId::Idle;
    switch (_boardState)
    {
    case BoardState::Start:
        break;
    case BoardState::Peek:
            firstStep = StepId::SeeOwnCard;
        break;
    case BoardState::Game:
    case BoardState::Cabo:
        firstStep = StepId::DrawCard;
        break;
    }
    return firstStep;
}

} // namespace cn::shared::game
