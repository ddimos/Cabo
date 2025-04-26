#include "client/game/step/DrawCard.hpp"

#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

DrawCard::DrawCard(Board& _board, PlayerId _managedPlayerId)
    : Step(_managedPlayerId,
        {
            {Id::WaitInput, {
                .onEnter = [this](){
                     if (m_boardRef.getLocalPlayerId() != getManagedPlayerId())
                        return;

                    m_boardRef.fillNotificationQueue("You can draw a card from the deck or the discard");
                },
            }},
            {Id::WaitCard, {            
                .onEnter = [this](){
                    if (m_boardRef.getLocalPlayerId() == getManagedPlayerId())
                    {
                        events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickPile, m_requestedCardFromDeck ? PileType::Deck : PileType::Discard);
                        m_boardRef.getContext().get<net::Manager>().send(event);
                    }           

                    Card* card = m_boardRef.drawCard(m_requestedCardFromDeck);

                    if (m_requestedCardFromDeck)
                    {
                        const auto* participant = m_boardRef.getParticipant(getManagedPlayerId());

                        card->changeState({ 
                            .desiredPosition = participant->getOpenCardPosition(),
                            .desiredRotation = participant->getOpenCardRotation(),
                            .desiredState = Card::State::Viewed,
                            .onFinishCallback = [](){}
                        });
                    }
                },
                .onUpdate = [this](sf::Time){
                    if (m_requestedCardFromDeck)
                    {
                        auto* card = m_boardRef.getDrawnCard();
                        if ((card->isCardValueValid() || m_boardRef.getLocalPlayerId() != getManagedPlayerId()) && !card->isTransiting())
                            requestFollowingState();
                    }
                    else
                    {
                        requestFollowingState();
                    }
                }
            }},
            {Id::Finished, {}}
        }
    )
    , m_boardRef(_board)
{
}

void DrawCard::processPlayerInput(InputType _inputType, InputDataVariant _data)
{
    if (_inputType == InputType::ClickPile)
    {
        if (getCurrentStateId() != Id::WaitInput)
            return;

        auto dataStruct = std::get<PileType>(_data);
        m_requestedCardFromDeck = dataStruct == PileType::Deck;

        requestFollowingState();
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
