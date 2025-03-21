#include "client/game/step/SwapCard.hpp"
#include "client/game/Participant.hpp"

#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"

#include "shared/net/Manager.hpp"

namespace cn::client::game::step
{

SwapCard::SwapCard(Board& _board, PlayerId _managedPlayerId, bool _seeCard)
    : Step(_managedPlayerId,
        {
            {Id::WaitClickSomeonesSlot, {}},
            {Id::ClickedSomeonesSlot, {
                .onEnter = [this](){
                    events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickSlot, ClickSlotInputData{m_slotId, m_slotOwnerId});
                    m_boardRef.getContext().get<net::Manager>().send(event);
                    
                    if (m_seeCard)
                    {
                        auto* owner = m_boardRef.getParticipant(m_slotOwnerId);
                        owner->onStartShowingOwnCardInSlot(m_slotId); // hide card in slot
                    }
                    else
                    {
                        requestState(Id::WaitClickOwnSlot);
                    }
                },
                .onUpdate = {}
            }},
            {Id::DecideSwap, {
                .onEnter = [this](){
                    auto* participant = m_boardRef.getParticipant(getManagedPlayerId());
                    participant->onStartShowingCard(Card(m_rank, m_suit));
                    
                    m_boardRef.onParticipantStartDecidingSwap();
                },
                .onUpdate = {}
            }},
            {Id::FinishDecidingSwap, {
                .onEnter = [this](){
                    events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::SwapDecision, m_isCardSwapped);
                    m_boardRef.getContext().get<net::Manager>().send(event);

                    m_boardRef.onParticipantFinishDecidingSwap();

                    if (m_isCardSwapped)
                        requestFollowingState();
                    else
                        requestState(Id::Finished);
                },
                .onUpdate = {}
            }},
            {Id::WaitClickOwnSlot, {
                .onEnter = {},
                .onUpdate = {}
            }},
            {Id::ClickedOwnSlot, {
                .onEnter = [this](){
                    events::RemotePlayerInputNetEvent event(getManagedPlayerId(), InputType::ClickSlot, ClickSlotInputData{m_ownSlotId, getManagedPlayerId()});
                    m_boardRef.getContext().get<net::Manager>().send(event);

                    requestFollowingState();
                },
                .onUpdate = {}
            }},
            {Id::Swap, {
                .onEnter = [this](){
                    // TODO Swap animation
                    requestFollowingState();
                },
            }},
            {Id::Finished, {
                .onEnter = [this](){
                    if (m_seeCard)
                    {
                        auto* owner = m_boardRef.getParticipant(m_slotOwnerId);
                        owner->onFinishShowingOwnCardInSlot(m_slotId);
                        
                        auto* participant = m_boardRef.getParticipant(getManagedPlayerId());
                        participant->onFinishShowingCard();
                    }
                },
                .onUpdate = {}
            }},
        })
    , m_boardRef(_board)
    , m_seeCard(_seeCard)
{
}

void SwapCard::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::LocalPlayerClickSlotEvent>(getListenerId(),
            [this](const events::LocalPlayerClickSlotEvent& _event)
            {
                if (getCurrentStateId() == Id::WaitClickSomeonesSlot)
                {
                    if (_event.slotOwnerId == getManagedPlayerId()) // TODO give feedback to the player
                       return;

                    m_slotId = _event.slotId;
                    m_slotOwnerId = _event.slotOwnerId;
                }
                else if (getCurrentStateId() == Id::WaitClickOwnSlot)
                {
                    if (_event.slotOwnerId != getManagedPlayerId()) // TODO give feedback to the player
                        return;
                    m_ownSlotId = _event.slotId;
                }
                else
                {
                    return;
                }

                requestFollowingState();
            }
        );
        _dispatcher.registerEvent<events::ProvideCardNetEvent>(getListenerId(),
            [this](const events::ProvideCardNetEvent& _event)
            {    
                if (getCurrentStateId() != Id::ClickedSomeonesSlot)
                    return;

                requestFollowingState();
                m_rank = _event.m_rank;
                m_suit = _event.m_suit;
            }
        );
        _dispatcher.registerEvent<events::LocalPlayerClickDecideSwapButtonEvent>(getListenerId(),
            [this](const events::LocalPlayerClickDecideSwapButtonEvent& _event)
            {    
                if (getCurrentStateId() != Id::DecideSwap)
                    return;

                m_isCardSwapped = _event.m_swap;
                requestFollowingState();
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::LocalPlayerClickSlotEvent>(getListenerId());
        _dispatcher.unregisterEvent<events::ProvideCardNetEvent>(getListenerId());
        _dispatcher.unregisterEvent<events::LocalPlayerClickDecideSwapButtonEvent>(getListenerId());
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
