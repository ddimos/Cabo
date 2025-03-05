#pragma once

#include "client/game/Board.hpp"
#include "client/game/Card.hpp"
#include "client/game/Step.hpp"

namespace cn::client::game::step
{

class TakeCard : public Step
{
public:
    TakeCard(Board& _board, PlayerId _managedPlayerId);

private:
    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;
    bool isFinished() const override;
    StepId getNextStepId() const override;
    
    enum Id : Step::StateId
    {
        WaitInput,
        Finished
    };

    Board& m_boardRef;

    ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;
};

} // namespace cn::client::game::step
