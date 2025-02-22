#pragma once

#include "client/game/Board.hpp"
#include "client/game/Step.hpp"

namespace cn::client::game::step
{

class DecideCard : public Step
{
public:
    DecideCard(Board& _board, PlayerId _managedPlayerId);

private:
    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;
    bool isFinished() const override;
    StepId getNextStepId() const override;

    enum Id : Step::StateId
    {
        WaitCard,
        WaitInput,
        Finished
    };

    Board& m_boardRef;
    DecideButton m_button = DecideButton::Action;
    Card::Ability m_cardAbility = Card::Ability::None;
};

} // namespace cn::client::game::step
