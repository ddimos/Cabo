#pragma once

#include "client/game/Board.hpp"
#include "client/game/Card.hpp"
#include "client/game/Step.hpp"

namespace cn::client::game::step
{

class TakeCard : public Step
{
public:
    TakeCard(Board& _board, PlayerId _managedPlayerId, Card& _takenCard);

private:
    void processPlayerInput(InputType _inputType, InputDataVariant _data) override;
    bool isFinished() const override;
    StepId getNextStepId() const override;
    
    enum Id : Step::StateId
    {
        WaitInput,
        Take,
        Finished
    };

    Board& m_boardRef;
    Card& m_takenCardRef;
    Card* m_discardedCardPtr;

    ParticipantSlotId m_slotId{};
};

} // namespace cn::client::game::step
