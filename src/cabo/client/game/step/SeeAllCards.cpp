#include "client/game/step/SeeAllCards.hpp"
// #include "client/game/Participant.hpp"

#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

SeeAllCards::SeeAllCards(Board& _board, PlayerId _managedPlayerId)
    : Step(_managedPlayerId,
        {
            {Id::WaitAllCardsAvailable, {
                .onEnter = [this](){},
                .onUpdate = [this](sf::Time){
                    auto* participant = m_boardRef.getParticipant(getManagedPlayerId());
                    bool allCardsValid = true;
                    participant->visitSlots(
                        [this, &allCardsValid](ParticipantSlot& _slot) {
                            if (!_slot.cardPtr->isCardValueValid())
                                allCardsValid = false;
                        }
                    );
                    if (allCardsValid)
                        requestFollowingState();
                }
            }},
            {Id::SeeCards, {            
                .onEnter = [this](){
                    auto* participant = m_boardRef.getParticipant(getManagedPlayerId());
                    participant->visitSlots(
                        [this](ParticipantSlot& _slot) {
                            _slot.cardPtr->show(true);
                        }
                    );
                },
                .onUpdate = [this](sf::Time _dt){
                    m_timeDt -= _dt;
                    if (m_timeDt.asSeconds() <= 0.f)
                       requestFollowingState();
                }
            }},
            {Id::Finished, {}}
        })
    , m_boardRef(_board)
{
}

bool SeeAllCards::isFinished() const
{
    return getCurrentStateId() == Id::Finished;
} 

StepId SeeAllCards::getNextStepId() const
{
    return StepId::FinishTurn;
}

} // namespace cn::client::game::step
