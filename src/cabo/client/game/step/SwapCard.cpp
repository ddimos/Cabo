#include "client/game/step/SwapCard.hpp"
#include "client/game/Participant.hpp"

#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

SwapCard::SwapCard(Board& _board, PlayerId _managedPlayerId, bool _seeCard)
    : Step(_managedPlayerId,
        {
            {Id::WaitClickSomeonesSlot, {
                .onEnter = [this](){
                    if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
                        m_boardRef.fillNotificationQueue("You can pick any card except yours");
                },
                .onUpdate = {}
            }},
            {Id::ClickedSomeonesSlot, {
                .onEnter = [this](){
                    const auto* viewer = m_boardRef.getParticipant(getManagedPlayerId());
                    const auto* owner = m_boardRef.getParticipant(m_ownerId);
                    const auto& slot = owner->getSlot(m_ownerSlotId);
                    m_takenCardPtr = slot.cardPtr;

                    m_takenCardPtr->changeState({ 
                        .desiredPosition = viewer->getOpenCardPosition(),
                        .desiredRotation = viewer->getOpenCardRotation(),
                        .desiredState = Card::State::Viewed,
                        .onFinishCallback = [](){}
                    });
                },
                .onUpdate = [this](sf::Time){
                    if ((m_takenCardPtr->isCardValueValid() || m_boardRef.getLocalPlayerId() != getManagedPlayerId() || !m_seeCard) && !m_takenCardPtr->isTransiting())
                    {
                        if (m_seeCard)
                            requestFollowingState();
                        else
                            requestState(Id::WaitClickOwnSlot);
                    }
                }
            }},
            {Id::DecideSwap, {
                .onEnter = [this](){
                    m_takenCardPtr->show(m_boardRef.getLocalPlayerId() == getManagedPlayerId());

                    if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
                        m_boardRef.showDecideSwapButtons();
                },
                .onUpdate = {}
            }},
            {Id::FinishDecidingSwap, {
                .onEnter = [this](){
                    if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
                        m_boardRef.hideDecideSwapButtons();

                    if (m_isCardSwapped)
                        requestFollowingState();
                    else
                    {
                        const auto* owner = m_boardRef.getParticipant(m_ownerId);
                        const auto& slot = owner->getSlot(m_ownerSlotId);

                        m_takenCardPtr->changeState({ 
                            .desiredPosition = slot.position,
                            .desiredRotation = slot.rotation,
                            .desiredState = Card::State::InHand,
                            .onFinishCallback = [](){}
                        });
                        requestState(Id::SwapOrReturn);
                    }
                },
                .onUpdate = {}
            }},
            {Id::WaitClickOwnSlot, {
                .onEnter = {},
                .onUpdate = {}
            }},
            {Id::ClickedOwnSlot, {
                .onEnter = [this](){
                    auto* owner = m_boardRef.getParticipant(m_ownerId);
                    auto& ownerSlot = owner->getSlot(m_ownerSlotId);
                    auto* viewer = m_boardRef.getParticipant(getManagedPlayerId());
                    auto& viewerSlot = viewer->getSlot(m_viewerSlotId);

                    m_takenCardPtr->changeState({ 
                        .desiredPosition = viewerSlot.position,
                        .desiredRotation = viewerSlot.rotation,
                        .desiredState = Card::State::InHand,
                        .onFinishCallback = [](){}
                    });

                    m_returnedCardPtr = viewerSlot.cardPtr;
                    viewerSlot.cardPtr = m_takenCardPtr;

                    m_returnedCardPtr->changeState({
                        .desiredPosition = ownerSlot.position,
                        .desiredRotation = ownerSlot.rotation,
                        .desiredState = Card::State::InHand,
                        .onFinishCallback = [](){}
                    });
                    ownerSlot.cardPtr = m_returnedCardPtr;

                    requestFollowingState();
                },
                .onUpdate = {}
            }},
            {Id::SwapOrReturn, {
                .onEnter = [this](){
                    m_takenCardPtr->hide();
                },
                .onUpdate = [this](sf::Time){
                    if (!m_takenCardPtr->isTransiting() && (m_returnedCardPtr == nullptr || !m_returnedCardPtr->isTransiting()))
                    {
                        requestFollowingState();
                    }
                }
            }},
            {Id::Finished, {}},
        })
    , m_boardRef(_board)
    , m_seeCard(_seeCard)
{
}

void SwapCard::processPlayerInput(InputType _inputType, InputDataVariant _data)
{
    if (_inputType == InputType::ClickSlot)
    {
        auto dataStruct = std::get<ClickSlotInputData>(_data);
        if (getCurrentStateId() == Id::WaitClickSomeonesSlot)
        {
            if (dataStruct.playerId == getManagedPlayerId()) // TODO give feedback to the player
                return;

            m_ownerSlotId = dataStruct.slotId;
            m_ownerId = dataStruct.playerId;

            if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
            {
                events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickSlot, ClickSlotInputData{m_ownerSlotId, m_ownerId});
                m_boardRef.getContext().get<net::Manager>().send(event);
            }

            requestFollowingState();
        }
        else if (getCurrentStateId() == Id::WaitClickOwnSlot)
        {
            if (dataStruct.playerId != getManagedPlayerId()) // TODO give feedback to the player
                return;

            m_viewerSlotId = dataStruct.slotId;

            if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
            {
                events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickSlot, ClickSlotInputData{m_viewerSlotId, getManagedPlayerId()});
                m_boardRef.getContext().get<net::Manager>().send(event);
            }

            requestFollowingState();
        }
    }
    else if (_inputType == InputType::SwapDecision)
    {
        if (getCurrentStateId() != Id::DecideSwap)
            return;

        m_isCardSwapped = std::get<bool>(_data);

        if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
        {
            events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::SwapDecision, m_isCardSwapped);
            m_boardRef.getContext().get<net::Manager>().send(event);
        }

        requestFollowingState();
    }
}

bool SwapCard::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId SwapCard::getNextStepId() const
{
    return StepId::FinishTurn;
}

} // namespace cn::client::game::step
