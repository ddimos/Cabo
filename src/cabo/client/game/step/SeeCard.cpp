#include "client/game/step/SeeCard.hpp"
#include "client/game/Participant.hpp"

#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

SeeCard::SeeCard(Board& _board, PlayerId _managedPlayerId, bool _seeOwnCard)
    : Step(_managedPlayerId,
        {
            {Id::WaitInput, {
                .onEnter = [this](){
                    if (m_boardRef.getLocalPlayerId() != getManagedPlayerId())
                        return;

                    if (m_seeOwnCard)
                        m_boardRef.fillNotificationQueue("You can peek any of your card");
                    else
                        m_boardRef.fillNotificationQueue("You can peek any card except yours");
                }
            }},
            {Id::RequestSeeCard, {            
                .onEnter = [this](){
                    const auto* viewer = m_boardRef.getParticipant(getManagedPlayerId());
                    const auto* owner = m_boardRef.getParticipant(m_slotOwnerId);
                    const auto& slot = owner->getSlot(m_slotId);

                    slot.cardPtr->changeState({ 
                        .desiredPosition = viewer->getOpenCardPosition(),
                        .desiredRotation = viewer->getOpenCardRotation(),
                        .desiredState = Card::State::Viewed,
                        .onFinishCallback = [](){}
                    });
                },
                .onUpdate = [this](sf::Time){
                    const auto* owner = m_boardRef.getParticipant(m_slotOwnerId);
                    const auto& slot = owner->getSlot(m_slotId);
                    
                    if ((slot.cardPtr->isCardValueValid() || m_boardRef.getLocalPlayerId() != getManagedPlayerId()) && !slot.cardPtr->isTransiting())
                        requestFollowingState();
                }
            }},
            {Id::View, {
                .onEnter = [this](){
                    const auto* owner = m_boardRef.getParticipant(m_slotOwnerId);
                    const auto& slot = owner->getSlot(m_slotId);

                    slot.cardPtr->show(m_boardRef.getLocalPlayerId() == getManagedPlayerId());
                },
                .onUpdate = [this](sf::Time _dt){
                    m_seeCardTimeDt -= _dt;
                    if (m_seeCardTimeDt.asSeconds() <= 0.f)
                       requestFollowingState();
                }
            }},
            {Id::WaitReturn, {
                .onEnter = [this](){
                    const auto* owner = m_boardRef.getParticipant(m_slotOwnerId);
                    const auto& slot = owner->getSlot(m_slotId);
    
                    slot.cardPtr->hide();
                    slot.cardPtr->changeState({ 
                        .desiredPosition = slot.position,
                        .desiredRotation = slot.rotation,
                        .desiredState = Card::State::InHand,
                        .onFinishCallback = [this](){
                            requestFollowingState();
                        }
                    });
                },
                .onUpdate = {}
            }},
            {Id::Finished, {}}
        })
    , m_boardRef(_board)
    , m_seeOwnCard(_seeOwnCard)
{
}

void SeeCard::processPlayerInput(InputType _inputType, InputDataVariant _data)
{
    if (_inputType == InputType::ClickSlot)
    {
        if (getCurrentStateId() != Id::WaitInput)
            return;
            
        auto dataStruct = std::get<ClickSlotInputData>(_data);
        if (m_seeOwnCard)
        {
            if (dataStruct.playerId != getManagedPlayerId()) // TODO give feedback to the player
                return;
        }
        else
        {
            if (dataStruct.playerId == getManagedPlayerId())
                return;
        }

        m_slotOwnerId = dataStruct.playerId;
        m_slotId = dataStruct.slotId;

        requestFollowingState();

        if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
        {
            events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickSlot, ClickSlotInputData{m_slotId, m_slotOwnerId});
            m_boardRef.getContext().get<net::Manager>().send(event);
        }
    }
}

bool SeeCard::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId SeeCard::getNextStepId() const
{
    return StepId::FinishTurn;
}

} // namespace cn::client::game::step
