#include "client/game/step/SeeOwnCard.hpp"
#include "client/game/Participant.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

SeeOwnCard::SeeOwnCard(Board& _board, PlayerId _localPlayerId)
    : shared::game::Step({
        {Id::WaitInput, {}},
        {Id::RequestSeeCard, {            
            .onEnter = [this](){
                auto* participant = m_boardRef.getParticipant(m_localPlayerId);
                participant->onStartShowingCardInSlot(m_slotId);
                
                events::RemotePlayerClickSlotEvent event(m_localPlayerId, m_slotId);
                m_boardRef.getContext().get<net::Manager>().send(event);
            },
            .onUpdate = [this](sf::Time){
            }
        }},
        {Id::SeeCard, {
            .onEnter = [this](){
                auto* participant = m_boardRef.getParticipant(m_localPlayerId);
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
                auto* participant = m_boardRef.getParticipant(m_localPlayerId);
                participant->onFinishShowingCardInSlot(m_slotId);
            },
            .onUpdate = [this](sf::Time){
                // for debugging
                // requestState(Id::WaitInput);
                // m_seeCardTimeDt = m_seeCardTime;
            }
        }},
    })
    , m_boardRef(_board)
    , m_localPlayerId(_localPlayerId)
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
                if (_event.slotOwnerId != m_localPlayerId) // TODO give feedback to the player
                    return;

                requestFollowingState();
                m_slotId = _event.slotId;
            }
        );
        _dispatcher.registerEvent<events::SeeCardInSlotEvent>(getListenerId(),
            [this](const events::SeeCardInSlotEvent& _event)
            {    
                if (getCurrentStateId() != Id::RequestSeeCard)
                    return;
                CN_ASSERT(_event.m_slotOwnerId == m_localPlayerId);
                CN_ASSERT(_event.m_slotId == m_slotId);

                requestFollowingState();
                m_rank = _event.m_rank;
                m_suit = _event.m_suit;
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::LocalPlayerClickSlotEvent>(getListenerId());
        _dispatcher.unregisterEvent<events::SeeCardInSlotEvent>(getListenerId());
    }
}

} // namespace cn::client::game::step
