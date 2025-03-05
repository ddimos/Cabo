#include "client/game/step/DrawCard.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

DrawCard::DrawCard(Board& _board, PlayerId _managedPlayerId)
    : Step(_managedPlayerId,
        {
            {Id::WaitInput, {}},
            {Id::RequestCard, {            
                .onEnter = [this](){                    
                    events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickPile, m_requestedCardFromDeck ? PileType::Deck : PileType::Discard);
                    m_boardRef.getContext().get<net::Manager>().send(event);

                    requestFollowingState();
                },
                .onUpdate = {}
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
        _dispatcher.registerEvent<events::LocalPlayerClickDeckEvent>(getListenerId(),
            [this](const events::LocalPlayerClickDeckEvent& _event)
            {    
                if (getCurrentStateId() != Id::WaitInput)
                    return;
                m_requestedCardFromDeck = true;
                requestFollowingState();
            }
        );
        _dispatcher.registerEvent<events::LocalPlayerClickDiscardEvent>(getListenerId(),
            [this](const events::LocalPlayerClickDiscardEvent& _event)
            {    
                if (getCurrentStateId() != Id::WaitInput)
                    return;
                m_requestedCardFromDeck = false;
                requestFollowingState();
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::LocalPlayerClickDeckEvent>(getListenerId());
        _dispatcher.unregisterEvent<events::LocalPlayerClickDiscardEvent>(getListenerId());
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

} // namespace cn::client::game::step
