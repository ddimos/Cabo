#pragma once

#include "client/game/Board.hpp"
#include "client/game/Step.hpp"

namespace cn::client::game::step
{

class DrawCard : public Step
{
public:
    DrawCard(Board& _board, PlayerId _managedPlayerId);

private:
    void processPlayerInput(InputType _inputType, InputDataVariant _data) override;
    bool isFinished() const override;
    StepId getNextStepId() const override;

    enum Id : Step::StateId
    {
        WaitInput,
        WaitCard,
        Finished
    };

    Board& m_boardRef;
    shared::game::ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;
    bool m_requestedCardFromDeck = true;
};

} // namespace cn::client::game::step
