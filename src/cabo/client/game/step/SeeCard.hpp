#pragma once

#include "client/game/Board.hpp"
#include "client/game/Step.hpp"

namespace cn::client::game::step
{

class SeeCard : public Step
{
public:
    SeeCard(Board& _board, PlayerId _managedPlayerId, bool _seeOwnCard);

private:
    void processPlayerInput(InputType _inputType, InputDataVariant _data) override;
    bool isFinished() const override;
    StepId getNextStepId() const override;
    
    enum Id : Step::StateId
    {
        WaitInput,
        RequestSeeCard,
        View,
        WaitReturn,
        Finished
    };

    Board& m_boardRef;
    const bool m_seeOwnCard = true;

    ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;
    PlayerId m_slotOwnerId{};

    const sf::Time m_seeCardTime = sf::seconds(2.f);
    sf::Time m_seeCardTimeDt = m_seeCardTime;
};

} // namespace cn::client::game::step
