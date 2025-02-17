#pragma once

#include "client/game/Board.hpp"
#include "client/game/Card.hpp"
#include "client/game/Step.hpp"
#include "shared/player/Types.hpp"

namespace cn::client::game::step
{

class SeeOwnCard : public Step
{
public:
    SeeOwnCard(Board& _board, PlayerId _localPlayerId);

private:
    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;

    enum Id : Step::StateId
    {
        WaitInput,
        RequestSeeCard,
        SeeCard,
        Finished
    };

    Board& m_boardRef;

    ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;
    PlayerId m_localPlayerId = PlayerIdInvalid;

    Card::Rank m_rank;
    Card::Suit m_suit;

    const sf::Time m_seeCardTime = sf::seconds(2.f);
    sf::Time m_seeCardTimeDt = m_seeCardTime;
};

} // namespace cn::client::game::step
