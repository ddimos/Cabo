#include "client/game/step/SeeCard.hpp"
#include "client/game/Participant.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

SeeCard::SeeCard(Board& _board, PlayerId _managedPlayerId, bool _seeOwnCard)
    : Step(_managedPlayerId,
        {
            {Id::WaitInput, {}},
            {Id::RequestSeeCard, {            
                .onEnter = [this](){
                    auto* participant = m_boardRef.getParticipant(m_slotOwnerId);
                    participant->onStartShowingCardInSlot(m_slotId);
                    
                    events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickSlot, ClickSlotInputData{m_slotId, m_slotOwnerId});
                    m_boardRef.getContext().get<net::Manager>().send(event);
                },
                .onUpdate = {}
            }},
            {Id::Look, {
                .onEnter = [this](){
                    auto* participant = m_boardRef.getParticipant(m_slotOwnerId);
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
                    auto* participant = m_boardRef.getParticipant(m_slotOwnerId);
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
    , m_seeOwnCard(_seeOwnCard)
{
}

void SeeCard::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::LocalPlayerClickSlotEvent>(getListenerId(),
            [this](const events::LocalPlayerClickSlotEvent& _event)
            {
                if (getCurrentStateId() != Id::WaitInput)
                    return;
                if (m_seeOwnCard)
                {
                    if (_event.slotOwnerId != getManagedPlayerId()) // TODO give feedback to the player
                        return;
                }
                else
                {
                    if (_event.slotOwnerId == getManagedPlayerId())
                        return;
                }

                requestFollowingState();
                m_slotId = _event.slotId;
                m_slotOwnerId = _event.slotOwnerId;
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

bool SeeCard::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId SeeCard::getNextStepId() const
{
    return StepId::FinishTurn;
}

} // namespace cn::client::game::step
