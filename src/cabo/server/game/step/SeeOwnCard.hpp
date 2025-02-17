#pragma once

#include "server/game/Board.hpp"
#include "server/game/Card.hpp"
#include "server/game/Step.hpp"
#include "server/game/Types.hpp"

#include "shared/player/Types.hpp"

namespace cn::server::game::step
{

class SeeOwnCard : public Step
{
public:
    SeeOwnCard(Board& _board, PlayerId _playerId);

private:
    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;

    enum Id: Step::StateId
    {
        WaitRequest,
        SendCard,
        Finished
    };

    Board& m_boardRef;

    PlayerId m_managedPlayerId = PlayerIdInvalid;
    
    ParticipantSlotId m_slotId = shared::game::ParticipantSlotIdInvalid;
    Card::Rank m_rank;
    Card::Suit m_suit;
};

} // namespace cn::server::game::step
