#pragma once

#include "client/game/Board.hpp"
#include "client/game/Card.hpp"
#include "client/game/Step.hpp"

namespace cn::client::game::step
{

class MatchCard : public Step
{
public:
    MatchCard(Board& _board, PlayerId _managedPlayerId);

private:
    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;
    bool isFinished() const override;
    StepId getNextStepId() const override;
    
    enum Id : Step::StateId
    {
        WaitInput,
        WaitServerReply,
        SeeCard,
        Finished
    };

    Board& m_boardRef;

    ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;

    Card::Rank m_rank;
    Card::Suit m_suit;
    bool m_isMatched = false;

    const sf::Time m_seeCardTime = sf::seconds(1.f);
    sf::Time m_seeCardTimeDt = m_seeCardTime;
};

} // namespace cn::client::game::step
