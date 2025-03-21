#pragma once

#include "client/game/Board.hpp"
#include "client/game/Card.hpp"
#include "client/game/Step.hpp"

namespace cn::client::game::step
{

class SwapCard : public Step
{
public:
    SwapCard(Board& _board, PlayerId _managedPlayerId, bool _seeCard);

private:
    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;
    bool isFinished() const override;
    StepId getNextStepId() const override;
    
    enum Id : Step::StateId
    {
        WaitClickSomeonesSlot,
        ClickedSomeonesSlot,
        DecideSwap,
        FinishDecidingSwap,
        WaitClickOwnSlot,
        ClickedOwnSlot,
        Swap,
        Finished
    };

    Board& m_boardRef;
    const bool m_seeCard = true;

    ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;
    PlayerId m_slotOwnerId = PlayerIdInvalid;
    ParticipantSlotId m_ownSlotId = shared::game::ParticipantSlotIdInvalid;

    Card::Rank m_rank;
    Card::Suit m_suit;

    bool m_isCardSwapped = false;
};

} // namespace cn::client::game::step
