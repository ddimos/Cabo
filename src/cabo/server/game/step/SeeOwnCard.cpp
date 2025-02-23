#include "server/game/step/SeeOwnCard.hpp"
#include "server/game/Participant.hpp"

#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"

#include "core/Log.hpp"

namespace cn::server::game::step
{

SeeOwnCard::SeeOwnCard(Board& _board, PlayerId _playerId)
    : Step(_playerId,
        { 
            {Id::WaitRequest, {}},
            {Id::SendCard, {            
                .onEnter = [this](){
                    auto* participant = m_boardRef.getParticipant(getManagedPlayerId());
                    auto* card = participant->getCard(m_slotId);

                    CN_LOG_FRM("See own card, participant: {}, slot: {}, card: {} {}", 
                        getManagedPlayerId(), m_slotId, (int)card->getRank(), (int)card->getSuit()
                    );
                    auto& netManRef = m_boardRef.getContext().get<net::Manager>();

                    {
                        events::SeeCardInSlotNetEvent event(getManagedPlayerId(), m_slotId, card->getRank(), card->getSuit());
                        netManRef.send(event, getManagedPlayerId());
                    }

                    {
                        events::RemotePlayerClickSlotNetEvent event(getManagedPlayerId(), m_slotId);
                        netManRef.send(event, nsf::MessageInfo::Type::EXCLUDE_BRODCAST, true, getManagedPlayerId());
                    }
                },
                .onUpdate = [this](sf::Time _dt){
                    m_seeCardTimeDt -= _dt;
                    if (m_seeCardTimeDt.asSeconds() <= 0.f)
                        requestFollowingState();
                }
            }},
            {Id::Finished, {
                .onEnter = [this](){
                },
                .onUpdate = [this](sf::Time){
                    // for debugging
                    // requestState(Id::WaitRequest);
                }
            }},
        })
    , m_boardRef(_board)
{
}

void SeeOwnCard::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::RemotePlayerClickSlotNetEvent>(getListenerId(),
            [this](const events::RemotePlayerClickSlotNetEvent& _event)
            {    
                if (getCurrentStateId() != Id::WaitRequest)
                    return;
                if (getManagedPlayerId() != _event.m_senderPeerId)
                    return;
                requestFollowingState();
                m_slotId = _event.m_slotId;
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::RemotePlayerClickSlotNetEvent>(getListenerId());
    }
}

bool SeeOwnCard::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId SeeOwnCard::getNextStepId() const
{
    return StepId::Idle;
}

} // namespace cn::server::game::step
