#include "client/game/step/TakeCard.hpp"
#include "client/game/Participant.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

TakeCard::TakeCard(Board& _board, PlayerId _managedPlayerId)
    : Step(_managedPlayerId,
        {
            {Id::WaitInput, {}},
            // TODO {Id::StartAnimation, {}} 
            {Id::Finished, {
                .onEnter = [this](){
                    auto* participant = m_boardRef.getParticipant(getManagedPlayerId());
                    participant->onProvideCardInSlot(m_slotId, m_boardRef.getDrawnCard());

                    events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickSlot, m_slotId);
                    m_boardRef.getContext().get<net::Manager>().send(event);
                },
                .onUpdate = {}
            }},
        })
    , m_boardRef(_board)
{
}

void TakeCard::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
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
    }
    else
    {
        _dispatcher.unregisterEvent<events::LocalPlayerClickSlotEvent>(getListenerId());
    }
}

bool TakeCard::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId TakeCard::getNextStepId() const
{
    return StepId::FinishTurn;
}

} // namespace cn::client::game::step
