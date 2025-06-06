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
    case BoardState::Finish:
        firstStep = StepId::SeeAllCards;
        break;
    }
    return firstStep;
}

inline StepId getNextStepId(ActionType _button, Card::Ability _ability)
{
    switch (_button)
    {
    case ActionType::Ability:
        switch (_ability)
        {
        case Card::Ability::Peek:
            return StepId::SeeOwnCard;
        case Card::Ability::Spy:
            return StepId::SeeSomeonesCard;
        case Card::Ability::SwapBlindly:
            return StepId::SwapCardBlindly;
        case Card::Ability::SwapOpenly:
            return StepId::SwapCardOpenly;
        default:
            break;
        }
    case ActionType::Discard:
        return StepId::FinishTurn;
    case ActionType::Match:
        return StepId::MatchCard;
    case ActionType::Take:
        return StepId::TakeCard;
    }
    return StepId::Idle;
}

} // namespace cn::shared::game
