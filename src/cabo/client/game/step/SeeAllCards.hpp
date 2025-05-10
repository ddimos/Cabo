#pragma once

#include "client/game/Board.hpp"
#include "client/game/Step.hpp"

namespace cn::client::game::step
{

class SeeAllCards : public Step
{
public:
    SeeAllCards(Board& _board, PlayerId _managedPlayerId);

private:
    void processPlayerInput(InputType _inputType, InputDataVariant _data) override {}
    bool isFinished() const override;
    StepId getNextStepId() const override;
    
    enum Id : Step::StateId
    {
        WaitAllCardsAvailable,
        SeeCards,
        Finished
    };

    Board& m_boardRef;

    const sf::Time m_time = sf::seconds(5.f);
    sf::Time m_timeDt = m_time;
};

} // namespace cn::client::game::step
