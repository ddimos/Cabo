#include "client/game/step/DecideCard.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

DecideCard::DecideCard(Board& _board, PlayerId _managedPlayerId)
    : Step(_managedPlayerId,
        {
            {Id::WaitCard, {         
                .onEnter = [this](){
                    // TODo
                },
                .onUpdate = [this](sf::Time){
                }
            }},
            {Id::WaitInput, {         
                .onEnter = [this](){
                    // TODo
                },
                .onUpdate = [this](sf::Time){
                }
            }},
            {Id::Finished, {}},
        }
    )
    , m_boardRef(_board)
{

}

void DecideCard::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::DrawCardNetEvent>(getListenerId(),
            [this](const events::DrawCardNetEvent& _event)
            {    
                if (getCurrentStateId() != Id::WaitInput)
                    return;

                requestFollowingState();
            }
        );
        _dispatcher.registerEvent<events::LocalPlayerClickDecideButtonEvent>(getListenerId(),
            [this](const events::LocalPlayerClickDecideButtonEvent& _event)
            {    
                if (getCurrentStateId() != Id::WaitInput)
                    return;
                m_button = _event.m_button;
                requestFollowingState();
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::DrawCardNetEvent>(getListenerId());
        _dispatcher.unregisterEvent<events::LocalPlayerClickDecideButtonEvent>(getListenerId());
    }
}

bool DecideCard::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId DecideCard::getNextStepId() const
{
    switch (m_button)
    {
    case DecideButton::Action:
        switch (m_cardAbility)
        {
        case Card::Ability::Peek:
            return StepId::SeeOwnCard;
        case Card::Ability::Spy:
            return StepId::SeeSomeonesCard;
        case Card::Ability::SwapBlindly:
            return StepId::SwapCardBlindly;
        case Card::Ability::SwapOpenly:
            return StepId::SwapCardOpenly;
        default:
            break;
        }
    case DecideButton::Discard:
        return StepId::DiscardCard;
    case DecideButton::Match:
        return StepId::MatchCard;
    case DecideButton::Take:
        return StepId::TakeCard;
    }
    return StepId::Idle;
}

} // namespace cn::client::game::step
