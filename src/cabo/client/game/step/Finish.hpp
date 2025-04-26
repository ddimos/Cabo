#pragma once

#include "client/game/Board.hpp"
#include "client/game/Step.hpp"

namespace cn::client::game::step
{

class Finish : public Step
{
public:
    Finish(Board& _board, PlayerId _managedPlayerId);

private:
    void processPlayerInput(InputType _inputType, InputDataVariant _data) override;
    bool isFinished() const override;
    StepId getNextStepId() const override;

    enum Id : Step::StateId
    {
        WaitInput,
        Finished
    };

    Board& m_boardRef;
    bool m_cabo = false;
};

} // namespace cn::client::game::step
