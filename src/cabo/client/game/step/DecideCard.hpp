#pragma once

#include "client/game/Board.hpp"
#include "client/game/Card.hpp"
#include "client/game/Step.hpp"

namespace cn::client::game::step
{

class DecideCard : public Step
{
public:
    DecideCard(Board& _board, PlayerId _managedPlayerId);

private:
    void processPlayerInput(InputType _inputType, InputDataVariant _data) override;
    bool isFinished() const override;
    StepId getNextStepId() const override;

    enum Id : Step::StateId
    {
        WaitInput,
        Discard,
        Finished
    };

    Board& m_boardRef;
    ActionType m_button = ActionType::Ability;

    Card::Rank m_rank;
    Card::Suit m_suit;
};

} // namespace cn::client::game::step
