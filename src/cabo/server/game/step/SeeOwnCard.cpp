#include "server/game/step/SeeOwnCard.hpp"
#include "server/game/Participant.hpp"

#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"

#include "core/Log.hpp"

namespace cn::server::game::step
{

SeeOwnCard::SeeOwnCard(Board& _board, PlayerId _playerId)
    : Step({
        {Id::WaitRequest, {}},
        {Id::SendCard, {            
            .onEnter = [this](){
                auto* participant = m_boardRef.getParticipant(m_managedPlayerId);
                auto* card = participant->getCard(m_slotId);

                CN_LOG_FRM("See own card, participant: {}, slot: {}, card: {} {}", 
                    m_managedPlayerId, m_slotId, (int)card->getRank(), (int)card->getSuit()
                );
                auto& netManRef = m_boardRef.getContext().get<net::Manager>();

                {
                    events::SeeCardInSlotEvent event(m_managedPlayerId, m_slotId, card->getRank(), card->getSuit());
                    netManRef.send(event, m_managedPlayerId);
                }

                {
                    events::RemotePlayerClickSlotEvent event(m_managedPlayerId, m_slotId);
                    netManRef.send(event, nsf::MessageInfo::Type::EXCLUDE_BRODCAST, true, m_managedPlayerId);
                }
            },
            .onUpdate = [this](sf::Time){
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
    , m_managedPlayerId(_playerId)
{
}

void SeeOwnCard::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::RemotePlayerClickSlotEvent>(getListenerId(),
            [this](const events::RemotePlayerClickSlotEvent& _event)
            {    
                if (getCurrentStateId() != Id::WaitRequest)
                    return;
                if (m_managedPlayerId != _event.m_senderPeerId)
                    return;
                requestFollowingState();
                m_slotId = _event.m_slotId;
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::RemotePlayerClickSlotEvent>(getListenerId());
    }
}

} // namespace cn::server::game::step
