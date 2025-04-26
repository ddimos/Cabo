#include "client/game/step/MatchCard.hpp"
#include "client/game/Participant.hpp"

#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

MatchCard::MatchCard(Board& _board, PlayerId _managedPlayerId, Card& _matchedCard)
    : Step(_managedPlayerId,
        {
            {Id::WaitInput, {
                .onEnter = [this](){
                    if (m_boardRef.getLocalPlayerId() != getManagedPlayerId())
                        return;

                    m_boardRef.fillNotificationQueue("You can pick any of your card");
                }
            }},
            {Id::WaitServerReply, {
                .onEnter = [this](){
                    if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
                    {
                        events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickSlot, ClickSlotInputData{m_slotId, getManagedPlayerId()});
                        m_boardRef.getContext().get<net::Manager>().send(event);
                    }

                    m_matchedCardRef.changeState({
                        .desiredPosition = m_boardRef.getCardPositions().discardPos,
                        .desiredRotation = 0.f,
                        .desiredState = Card::State::InDiscard,
                        .onFinishCallback = [](){}
                    });
                    m_boardRef.preDiscardCard(&m_matchedCardRef);

                    auto* owner = m_boardRef.getParticipant(getManagedPlayerId());
                    const auto& slot = static_cast<const Participant*>(owner)->getSlot(m_slotId);
                    m_secondMatchedCardPtr = slot.cardPtr;
                    m_secondMatchedCardPtr->changeState({
                        .desiredPosition = m_boardRef.getCardPositions().firstMatchPos,
                        .desiredRotation = 0.f,
                        .desiredState = Card::State::Matched,
                        .onFinishCallback = [](){}
                    });
                },
                .onUpdate = [this](sf::Time){
                    bool isReadyForNewState = true;
                    if (!m_matchedCardRef.isCardValueValid() || m_matchedCardRef.isTransiting())
                        isReadyForNewState = false;
                    if (!m_secondMatchedCardPtr->isCardValueValid() || m_secondMatchedCardPtr->isTransiting())
                        isReadyForNewState = false;

                    if (isReadyForNewState)
                        requestFollowingState();
                }
            }},
            {Id::SeeCard, {
                .onEnter = [this](){
                    m_matchedCardRef.show(true);
                    m_secondMatchedCardPtr->show(true);
                    m_boardRef.discardCard(&m_matchedCardRef);
                },
                .onUpdate = [this](sf::Time _dt){
                    m_seeCardTimeDt -= _dt;
                    // TODO Maybe I don't need to wait for the end in case of a successful match  
                    if (m_seeCardTimeDt.asSeconds() <= 0.f)
                        requestFollowingState();
                }
            }},
            {Id::ReturnCard, {
                .onEnter = [this](){
                    if (m_matchedCardRef.getRank() == m_secondMatchedCardPtr->getRank())
                    {
                        m_boardRef.discardCard(m_secondMatchedCardPtr);
                        auto* participant = m_boardRef.getParticipant(getManagedPlayerId());

                        m_secondMatchedCardPtr->changeState({
                            .desiredPosition = m_boardRef.getCardPositions().discardPos,
                            .desiredRotation = 0.f,
                            .desiredState = Card::State::InDiscard,
                            .onFinishCallback = [](){}
                        });
                    }
                    else
                    {
                        auto* participant = m_boardRef.getParticipant(getManagedPlayerId());
                        const auto& slot = static_cast<const Participant*>(participant)->getSlot(m_slotId);

                        m_secondMatchedCardPtr->changeState({
                            .desiredPosition = slot.position,
                            .desiredRotation = slot.rotation,
                            .desiredState = Card::State::InHand,
                            .onFinishCallback = [this](){
                                m_secondMatchedCardPtr->hide();
                            }
                        });
                    }
                },
                .onUpdate = [this](sf::Time){
                    if (!m_secondMatchedCardPtr->isTransiting())
                    {
                        requestFollowingState();
                    }
                }
            }},
            {Id::Finished, {}}
        })
    , m_boardRef(_board)
    , m_matchedCardRef(_matchedCard)
{
}

void MatchCard::processPlayerInput(InputType _inputType, InputDataVariant _data)
{
    if (_inputType == InputType::ClickSlot)
    {
        if (getCurrentStateId() != Id::WaitInput)
            return;

        auto dataStruct = std::get<ClickSlotInputData>(_data);
        if (dataStruct.playerId != getManagedPlayerId()) // TODO give feedback to the player
            return;

        m_slotId = dataStruct.slotId;
        requestFollowingState();
    }
}

bool MatchCard::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId MatchCard::getNextStepId() const
{
    return StepId::FinishTurn;
}

} // namespace cn::client::game::step
