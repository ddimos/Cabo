#include "client/game/step/TakeCard.hpp"
#include "client/game/Participant.hpp"

#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

TakeCard::TakeCard(Board& _board, PlayerId _managedPlayerId, Card& _takenCard)
    : Step(_managedPlayerId,
        {
            {Id::WaitInput, {
                .onEnter = [this](){
                    if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
                        m_boardRef.fillNotificationQueue("You can pick any of your card");
                },
                .onUpdate = {}
            }},
            {Id::Take, {
                .onEnter = [this](){
                    auto* owner = m_boardRef.getParticipant(getManagedPlayerId());
                    const auto& slot = static_cast<const Participant*>(owner)->getSlot(m_slotId);
    
                    m_takenCardRef.hide();
                    m_takenCardRef.changeState({
                        .desiredPosition = slot.position,
                        .desiredRotation = slot.rotation,
                        .desiredState = Card::State::InHand,
                        .onFinishCallback = [](){}
                    });
                    m_discardedCardPtr = owner->replaceCardInSlot(m_slotId, &m_takenCardRef);

                    m_discardedCardPtr->changeState({ 
                        .desiredPosition = m_boardRef.getCardPositions().discardPos,
                        .desiredRotation = 0.f,
                        .desiredState = Card::State::InDiscard,
                        .onFinishCallback = [](){}
                    });
                    m_boardRef.preDiscardCard(m_discardedCardPtr);
                },
                .onUpdate = [this](sf::Time){
                    if (!m_takenCardRef.isTransiting() && !m_discardedCardPtr->isTransiting())
                        requestFollowingState();
                }
            }},
            {Id::Finished, {
                .onEnter = [this](){
                    m_boardRef.discardCard(m_discardedCardPtr);
                },
                .onUpdate = {}
            }},
        })
    , m_boardRef(_board)
    , m_takenCardRef(_takenCard)
{
}

void TakeCard::processPlayerInput(InputType _inputType, InputDataVariant _data)
{
    if (_inputType == InputType::ClickSlot)
    {
        if (getCurrentStateId() != Id::WaitInput)
            return;

        auto dataStruct = std::get<ClickSlotInputData>(_data);
        if (dataStruct.playerId != getManagedPlayerId()) // TODO give feedback to the player
            return;
        
        requestFollowingState();
        m_slotId = dataStruct.slotId;

        if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
        {
            events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickSlot, ClickSlotInputData{m_slotId, getManagedPlayerId()});
            m_boardRef.getContext().get<net::Manager>().send(event);
        }
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
