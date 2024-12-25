#pragma once

#include "game/step/Step.hpp"
#include "game/Board.hpp"

namespace cn::game::step
{

class SeeOwnCard : public Step
{
public:
    SeeOwnCard(Board& _board);

private:
    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;
    void update(sf::Time _dt) override;

    enum class State
    {
        WaitInput,
        RequestSeeCard,
        SeeCard,
        Finished
    };
    State m_state = State::WaitInput;
    State m_nextState = State::WaitInput;

    Board& m_boardRef;

    game::PlayerSlotId m_slotId = game::PlayerSlotIdInvalid;
    game::PlayerId m_playerId = game::PlayerIdInvalid;

    const sf::Time m_seeCardTime = sf::seconds(2.f);
    sf::Time m_seeCardTimeDt = m_seeCardTime;
};

} // namespace cn::game::step
