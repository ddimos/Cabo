#include "client/game/step/DecideCard.hpp"

#include "shared/game/Board.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

DecideCard::DecideCard(Board& _board, PlayerId _managedPlayerId)
    : Step(_managedPlayerId,
        {
            {Id::WaitInput, {         
                .onEnter = [this](){
                    bool local = m_boardRef.getLocalPlayerId() == getManagedPlayerId();
                    m_boardRef.getDrawnCard()->show(local);
                    if (local)
                    {
                        m_boardRef.showDecideActionButtons();
                        m_boardRef.fillNotificationQueue("Decide the card");
                    }
                },
                .onUpdate = {}
            }},
            {Id::Discard, {
                .onEnter = [this](){
                    Card* card = m_boardRef.getDrawnCard();
                    if (m_button == ActionType::Discard || m_button == ActionType::Ability)
                    {
                        card->changeState({ 
                            .desiredPosition = m_boardRef.getCardPositions().discardPos,
                            .desiredRotation = 0.f,
                            .desiredState = Card::State::InDiscard,
                            .onFinishCallback = [](){}
                        });
                        m_boardRef.preDiscardCard(card);
                    }
                    else
                    {
                        requestFollowingState();
                    }
                },
                .onUpdate = [this](sf::Time){
                    if (m_button != ActionType::Discard && m_button != ActionType::Ability)
                        return;

                    Card* card = m_boardRef.getDrawnCard();
                    if (!card->isTransiting() && card->isCardValueValid())
                    {
                        m_boardRef.discardCard(card);
                        requestFollowingState();
                    }
                }
            }},
            {Id::Finished, {
                .onEnter = [this](){
                    if (m_boardRef.getLocalPlayerId() != getManagedPlayerId())
                        return;

                    m_boardRef.hideDecideActionButtons();

                },
                .onUpdate = {}
            }},
        }
    )
    , m_boardRef(_board)
{
}

void DecideCard::processPlayerInput(InputType _inputType, InputDataVariant _data)
{
    if (_inputType == InputType::Action)
    {
        if (getCurrentStateId() != Id::WaitInput)
            return;

        m_button = std::get<ActionType>(_data);
    
        if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
        {
            if (m_button == shared::game::ActionType::Ability && !m_boardRef.getDrawnCard()->hasAbility())
                return;

            events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::Action, m_button);
            m_boardRef.getContext().get<net::Manager>().send(event);
        }
        requestFollowingState();
    }
}

bool DecideCard::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId DecideCard::getNextStepId() const
{
    return shared::game::getNextStepId(m_button, m_boardRef.getDrawnCard()->getAbility());
}

} // namespace cn::client::game::step
