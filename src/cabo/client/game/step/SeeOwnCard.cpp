#include "client/game/step/SeeOwnCard.hpp"
#include "client/game/Participant.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

SeeOwnCard::SeeOwnCard(Board& _board, PlayerId _managedPlayerId)
    : Step(_managedPlayerId,
        {
            {Id::WaitInput, {}},
            {Id::RequestSeeCard, {            
                .onEnter = [this](){
                    auto* participant = m_boardRef.getParticipant(getManagedPlayerId());
                    participant->onStartShowingCardInSlot(m_slotId);
                    
                    events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickSlot, m_slotId);
                    m_boardRef.getContext().get<net::Manager>().send(event);
                },
                .onUpdate = {}
            }},
            {Id::SeeCard, {
                .onEnter = [this](){
                    auto* participant = m_boardRef.getParticipant(getManagedPlayerId());
                    participant->onCardRecievedInSlot(m_slotId, Card(m_rank, m_suit));
                },
                .onUpdate = [this](sf::Time _dt){
                    m_seeCardTimeDt -= _dt;
                    if (m_seeCardTimeDt.asSeconds() <= 0.f)
                        requestFollowingState();
                }
            }},
            {Id::Finished, {
                .onEnter = [this](){
                    auto* participant = m_boardRef.getParticipant(getManagedPlayerId());
                    participant->onFinishShowingCardInSlot(m_slotId);
                },
                .onUpdate = {}
                //     [this](sf::Time){
                //     // for debugging
                //     // requestState(Id::WaitInput);
                //     // m_seeCardTimeDt = m_seeCardTime;
                // }
            }},
        })
    , m_boardRef(_board)
{
}

void SeeOwnCard::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::LocalPlayerClickSlotEvent>(getListenerId(),
            [this](const events::LocalPlayerClickSlotEvent& _event)
            {
                if (getCurrentStateId() != Id::WaitInput)
                    return;
                if (_event.slotOwnerId != getManagedPlayerId()) // TODO give feedback to the player
                    return;

                requestFollowingState();
                m_slotId = _event.slotId;
            }
        );
        _dispatcher.registerEvent<events::ProvideCardNetEvent>(getListenerId(),
            [this](const events::ProvideCardNetEvent& _event)
            {    
                if (getCurrentStateId() != Id::RequestSeeCard)
                    return;

                requestFollowingState();
                m_rank = _event.m_rank;
                m_suit = _event.m_suit;
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::LocalPlayerClickSlotEvent>(getListenerId());
        _dispatcher.unregisterEvent<events::ProvideCardNetEvent>(getListenerId());
    }
}

bool SeeOwnCard::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId SeeOwnCard::getNextStepId() const
{
    return StepId::FinishTurn;
}

} // namespace cn::client::game::step
