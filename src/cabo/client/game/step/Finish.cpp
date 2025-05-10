#include "client/game/step/Finish.hpp"
#include "client/game/Participant.hpp"

#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

Finish::Finish(Board& _board, PlayerId _managedPlayerId)
    : Step(_managedPlayerId,
        {
            {Id::WaitInput, {
                .onEnter = [this](){
                    if (m_boardRef.getLocalPlayerId() != getManagedPlayerId())
                        return;

                    m_boardRef.fillNotificationQueue("Finish or cabo");
                    if (m_boardRef.canSayCabo())
                        m_boardRef.showCaboButton();
                    m_boardRef.showFinishButton();
                },
                .onUpdate = {}
            }},
            {Id::Finished, {
                .onEnter = [this](){
                    if (m_boardRef.getLocalPlayerId() != getManagedPlayerId())
                        return;

                    if (m_boardRef.canSayCabo())
                        m_boardRef.hideCaboButton();
                    m_boardRef.hideFinishButton();
                },
                .onUpdate = {}
            }}
        }
    )
    , m_boardRef(_board)
{
}

void Finish::processPlayerInput(InputType _inputType, InputDataVariant _data)
{
    if (_inputType == InputType::Finish || _inputType == InputType::Cabo)
    {
        if (getCurrentStateId() != Id::WaitInput)
            return;
        
        m_cabo = _inputType == InputType::Cabo;

        if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
        {
            events::RemotePlayerInputNetEvent event(getManagedPlayerId(), m_cabo ? InputType::Cabo : InputType::Finish);
            m_boardRef.getContext().get<net::Manager>().send(event);
        }

        requestFollowingState();
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
