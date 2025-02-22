#pragma once

#include "server/game/Board.hpp"
#include "server/game/Step.hpp"

namespace cn::server::game::step
{

class DrawCard : public Step
{
public:
    DrawCard(Board& _board, PlayerId _managedPlayerId);

private:
    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;
    bool isFinished() const override;
    StepId getNextStepId() const override;

    enum Id : Step::StateId
    {
        WaitRequest,
        SendCard,
        Finished
    };

    Board& m_boardRef;
    shared::game::ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;
    bool m_requestedCardFromDeck = true;
};

} // namespace cn::server::game::step
