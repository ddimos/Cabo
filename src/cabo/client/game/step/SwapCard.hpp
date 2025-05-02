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
    void processPlayerInput(InputType _inputType, InputDataVariant _data) override;
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
        SwapOrReturn,
        Finished
    };

    Board& m_boardRef;
    const bool m_seeCard = true;

    ParticipantSlotId m_ownerSlotId{};
    PlayerId m_ownerId{};
    ParticipantSlotId m_viewerSlotId{};

    Card* m_takenCardPtr = nullptr;
    Card* m_returnedCardPtr = nullptr;

    bool m_isCardSwapped = false;
};

} // namespace cn::client::game::step
