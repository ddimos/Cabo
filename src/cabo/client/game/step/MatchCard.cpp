#include "client/game/step/MatchCard.hpp"

#include "client/game/Participant.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

MatchCard::MatchCard(Board& _board, PlayerId _managedPlayerId)
    : Step(_managedPlayerId,
        {
            {Id::WaitInput, {}},
            {Id::WaitServerReply, {
                .onEnter = [this](){
                    events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickSlot, ClickSlotInputData{m_slotId, getManagedPlayerId()});
                    m_boardRef.getContext().get<net::Manager>().send(event);

                    auto* participant = m_boardRef.getParticipant(getManagedPlayerId());
                    participant->onStartShowingOwnCardInSlot(m_slotId);
                },
                .onUpdate = {}
            }},
            {Id::SeeCard, {
                .onEnter = [this](){
                    m_boardRef.onShowMatchedCard(Card(m_rank, m_suit));
                },
                .onUpdate = [this](sf::Time _dt){
                    m_seeCardTimeDt -= _dt;
                    // TODO Maybe I don't need to wait for the end in case of a successful match  
                    if (m_seeCardTimeDt.asSeconds() <= 0.f)
                        requestFollowingState();
                }
            }},
            {Id::Finished, {
                .onEnter = [this](){
                    auto* participant = m_boardRef.getParticipant(getManagedPlayerId());
                    participant->onFinishShowingOwnCardInSlot(m_slotId);

                    m_boardRef.onHideMatchedCard(m_isMatched);
                },
                .onUpdate = {}
            }},
        })
    , m_boardRef(_board)
{
}

void MatchCard::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
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
        _dispatcher.registerEvent<events::MatchCardNetEvent>(getListenerId(),
            [this](const events::MatchCardNetEvent& _event)
            {    
                if (getCurrentStateId() != Id::WaitServerReply)
                    return;

                requestFollowingState();
                m_rank = _event.m_rank;
                m_suit = _event.m_suit;
                m_isMatched = _event.m_isMatched;
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::LocalPlayerClickSlotEvent>(getListenerId());
        _dispatcher.unregisterEvent<events::MatchCardNetEvent>(getListenerId());
    }
}

bool MatchCard::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId MatchCard::getNextStepId() const
{
    return StepId::FinishTurn;
}

} // namespace cn::client::game::step
