#include "server/game/step/DrawCard.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::server::game::step
{

DrawCard::DrawCard(Board& _board, PlayerId _managedPlayerId)
    : Step(_managedPlayerId,
        {
            {Id::WaitRequest, {}},
            {Id::SendCard, {            
                .onEnter = [this](){                   
                    events::RemotePlayerClickPileNetEvent event(m_requestedCardFromDeck);
                    m_boardRef.getContext().get<net::Manager>().send(event);

                    requestFollowingState();
                },
                .onUpdate = [this](sf::Time){
                }
            }},
            {Id::Finished, {}}
        }
    )
    , m_boardRef(_board)
{
}

void DrawCard::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::RemotePlayerClickPileNetEvent>(getListenerId(),
            [this](const events::RemotePlayerClickPileNetEvent& _event)
            {    
                if (getCurrentStateId() != Id::WaitRequest)
                    return;
                if (getManagedPlayerId() != _event.m_senderPeerId)
                    return;
                m_requestedCardFromDeck = true;
                requestFollowingState();
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::RemotePlayerClickPileNetEvent>(getListenerId());
    }
}

bool DrawCard::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId DrawCard::getNextStepId() const
{
    return m_requestedCardFromDeck 
        ? StepId::DecideCard
        : StepId::TakeCard;
}

} // namespace cn::server::game::step
