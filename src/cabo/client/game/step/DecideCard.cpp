#include "client/game/step/DecideCard.hpp"

#include "shared/game/Board.hpp"

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
                    // TODO start a preview animation
                },
                .onUpdate = {}
            }},
            {Id::WaitInput, {         
                .onEnter = [this](){
                    m_boardRef.onParticipantStartDeciding(Card(m_rank, m_suit));
                },
                .onUpdate = {}
            }},
            {Id::Finished, {
                .onEnter = [this](){
                    m_boardRef.onParticipantFinishDeciding();
                    // TODO Shouldn't I send the result of deciding
                    events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::DecideButton, m_button);
                    m_boardRef.getContext().get<net::Manager>().send(event);
                },
                .onUpdate = {}
            }},
        }
    )
    , m_boardRef(_board)
{

}

void DecideCard::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::ProvideCardNetEvent>(getListenerId(),
            [this](const events::ProvideCardNetEvent& _event)
            {    
                if (getCurrentStateId() != Id::WaitCard)
                    return;

                m_rank = _event.m_rank;
                m_suit = _event.m_suit;

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
        _dispatcher.unregisterEvent<events::ProvideCardNetEvent>(getListenerId());
        _dispatcher.unregisterEvent<events::LocalPlayerClickDecideButtonEvent>(getListenerId());
    }
}

bool DecideCard::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId DecideCard::getNextStepId() const
{
    return shared::game::getNextStepId(m_button, m_cardAbility);
}

} // namespace cn::client::game::step
