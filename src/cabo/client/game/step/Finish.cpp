#include "client/game/step/Finish.hpp"
#include "client/game/Participant.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{


Finish::Finish(Board& _board, PlayerId _managedPlayerId)
    : Step(_managedPlayerId,
        {
            {Id::WaitInput, {
                .onEnter = [this](){                    
                    m_boardRef.onParticipantFinishesTurn(getManagedPlayerId());
                },
                .onUpdate = {}
            }},
            {Id::Finished, {            
                .onEnter = [this](){                    
                    events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::Finish);
                    m_boardRef.getContext().get<net::Manager>().send(event);

                    m_boardRef.onParticipantFinishedTurn(getManagedPlayerId());
                },
                .onUpdate = {}
            }}
        }
    )
    , m_boardRef(_board)
{
}

void Finish::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::LocalPlayerClickFinishButtonEvent>(getListenerId(),
            [this](const events::LocalPlayerClickFinishButtonEvent& _event)
            {    
                if (getCurrentStateId() != Id::WaitInput)
                    return;
                requestFollowingState();
            }
        );

        // TODO cabo button
    }
    else
    {
        _dispatcher.unregisterEvent<events::LocalPlayerClickFinishButtonEvent>(getListenerId());
    }
}

bool Finish::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId Finish::getNextStepId() const
{
    return StepId::Idle;
}

} // namespace cn::client::game::step
