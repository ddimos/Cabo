#include "client/game/Board.hpp"
#include "client/game/SpriteSheet.hpp"
#include "client/game/Types.hpp"

#include "client/game/step/DecideCard.hpp"
#include "client/game/step/DrawCard.hpp"
#include "client/game/step/Finish.hpp"
#include "client/game/step/MatchCard.hpp"
#include "client/game/step/SeeCard.hpp"
#include "client/game/step/SwapCard.hpp"
#include "client/game/step/TakeCard.hpp"

#include "client/player/Manager.hpp"

#include "shared/game/Board.hpp"
#include "shared/events/GameEvents.hpp"
#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"

#include "core/Log.hpp"

namespace cn::client::game
{

Board::Board(const core::Context& _context, std::vector<game::Participant*>&& _participants, std::vector<Card*>&& _cards, 
             menu::item::NotificationQueue& _queue, menu::item::Button& _finishButton, DecideActionButtons&& _decideActionButtons,
             DecideSwapButtons&& _decideSwapButtons)
    : m_contextRef(_context)
    , m_participants(std::move(_participants))
    , m_cards(std::move(_cards))
    , m_queueRef(_queue)
    , m_finishButtonRef(_finishButton)
    , m_decideActionButtons(std::move(_decideActionButtons))
    , m_decideSwapButtons(std::move(_decideSwapButtons))
{
    auto& eventDispatcherRef = getContext().get<core::event::Dispatcher>();
    auto& playerManagerRef = getContext().get<player::Manager>();

    m_localPlayerId = playerManagerRef.getLocalPlayerId();

    m_listenerId = core::event::getNewListenerId();
}

void Board::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::BoardStateUpdateNetEvent>(m_listenerId,
            [&_dispatcher, this](const events::BoardStateUpdateNetEvent& _event){
                CN_LOG_FRM("Board {} ", (unsigned)_event.m_boardState);
                // m_queueRef.push("Board state changed"); // TODO give a name
                m_boardState = _event.m_boardState;
            }
        );
        _dispatcher.registerEvent<events::PlayerTurnUpdateNetEvent>(m_listenerId,
            [&_dispatcher, this](const events::PlayerTurnUpdateNetEvent& _event){
                CN_LOG_FRM("Turn {} started {}", (unsigned)_event.m_playerId, _event.m_hasTurnStarted);
                
                if (_event.m_playerId == m_localPlayerId)
                    m_queueRef.push("Your turn");   
                else
                    m_queueRef.push("Remote player starts turn");
                
                CN_ASSERT(_event.m_hasTurnStarted); // TODO to implement the turn timeout

                if (!m_steps.contains(_event.m_playerId))
                    m_steps.emplace(_event.m_playerId, std::unique_ptr<Step>());

                StepId nextStepId = shared::game::getFirstStepId(m_boardState);
                changeStep(_event.m_playerId, nextStepId, m_steps.at(_event.m_playerId));
            }
        );
        _dispatcher.registerEvent<events::RemotePlayerInputNetEvent>(m_listenerId,
            [this](const events::RemotePlayerInputNetEvent& _event){
                auto it = m_steps.find(_event.m_playerId);
                if (it != m_steps.end())
                    it->second->processPlayerInput(_event.m_inputType, _event.m_data);
            }
        );
        _dispatcher.registerEvent<events::LocalPlayerClickSlotEvent>(m_listenerId,
            [this](const events::LocalPlayerClickSlotEvent& _event)
            {
                auto it = m_steps.find(m_localPlayerId);
                if (it != m_steps.end())
                    it->second->processPlayerInput(InputType::ClickSlot, ClickSlotInputData{_event.slotId, _event.slotOwnerId});
            }
        );
        _dispatcher.registerEvent<events::ProvideCardNetEvent>(m_listenerId,
            [&_dispatcher, this](const events::ProvideCardNetEvent& _event){
                auto* owner = getParticipant(_event.m_slotOwnerId);
                owner->setCardInSlot(_event.m_slotId, _event.m_rank, _event.m_suit);
            }
        );
        _dispatcher.registerEvent<events::DiscardCardNetEvent>(m_listenerId,
            [&_dispatcher, this](const events::DiscardCardNetEvent& _event){
                // discardCard(Card(_event.m_rank, _event.m_suit));
            }
        );
        _dispatcher.registerEvent<events::PlayerSlotUpdateNetEvent>(m_listenerId,
            [&_dispatcher, this](const events::PlayerSlotUpdateNetEvent& _event){
                auto* participant = getParticipant(_event.m_playerId);
                if (_event.m_wasAdded)
                {
                    participant->addSlot(_event.m_slotId);
                    m_queueRef.push("Player gets a card");
                }
                else
                {
                    participant->removeSlot(_event.m_slotId);
                    m_queueRef.push("Player gets rid of a card");
                }
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::BoardStateUpdateNetEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::PlayerTurnUpdateNetEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::RemotePlayerInputNetEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::LocalPlayerClickSlotEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::ProvideCardNetEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::DiscardCardNetEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::PlayerSlotUpdateNetEvent>(m_listenerId);
    }
}

void Board::update(sf::Time _dt)
{
    switch (m_boardState)
    {
    case BoardState::Start:
    {
        // TODO remove this bool
        if (!m_isDealt)
        {
            m_isDealt = true;
            for (auto* participant : m_participants)
            {
                participant->visitSlots(
                    [this, participant](ParticipantSlot& _slot) {
                        Card* card = getNextCard();
                        _slot.cardPtr = card;
                        card->changeState({ 
                            .desiredPosition = _slot.position,
                            .desiredRotation = _slot.rotation,
                            .desiredState = Card::State::InHand,
                            .onFinishCallback = [&_slot](){
                                _slot.buttonRef.requestActivated();
                            }
                        });
                    }
                );
            }
        }
    }
    }
    for (auto& [id, step] : m_steps)
    {
        if (!step)
            continue;

        step->update(_dt);
        if (step->isFinished())
        {
            StepId nextStepId = step->getNextStepId();
            changeStep(id, nextStepId, step);
        }
    }
}

Participant* Board::getParticipant(PlayerId _id)
{
    for (auto* participant : m_participants)
    {
        if (participant->getId() == _id)
            return participant;
    }
    CN_ASSERT_FRM(false, "Couldn't find a participant {}", _id);
    return nullptr;
}

void Board::onParticipantStartDeciding(Card* _card)
{
    m_drawnCardPtr = _card;

    for (auto& button : m_decideActionButtons)
        button->requestActivated();
}

void Board::onParticipantFinishDeciding()
{
    for (auto& button : m_decideActionButtons)
        button->requestDeactivated();
}

void Board::onParticipantStartDecidingSwap()
{
    for (auto& button : m_decideSwapButtons)
        button->requestActivated();
}

void Board::onParticipantFinishDecidingSwap()
{
    for (auto& button : m_decideSwapButtons)
        button->requestDeactivated();
}

void Board::onParticipantFinishesTurn(PlayerId _id)
{
    CN_ASSERT(_id == m_localPlayerId); // TODO
    m_finishButtonRef.requestActivated();
}

void Board::onParticipantFinishedTurn(PlayerId _id)
{
    CN_ASSERT(_id == m_localPlayerId); // TODO
    m_finishButtonRef.requestDeactivated();
}

void Board::onShowMatchedCard(Card* _card)
{
    m_matchedCardPtr = _card;
}

void Board::onHideMatchedCard(bool _discard)
{
    if (_discard)
        discardCard(m_matchedCardPtr);
}

void Board::fillNotificationQueue(const std::string& _message)
{
    m_queueRef.push(_message);
}

void Board::changeStep(PlayerId _playerId, StepId _nextStepId, std::unique_ptr<Step>& _step)
{
    switch (_nextStepId)
    {
    case StepId::Idle:
        _step.reset();
        break;
    case StepId::DecideCard:
        _step = std::make_unique<step::DecideCard>(*this, _playerId);
        break;
    case StepId::DrawCard:
        _step = std::make_unique<step::DrawCard>(*this, _playerId);
        break;
    case StepId::FinishTurn:
        _step = std::make_unique<step::Finish>(*this, _playerId);
        break;
    case StepId::MatchCard:
        _step = std::make_unique<step::MatchCard>(*this, _playerId);
        break;
    case StepId::SeeOwnCard:
        _step = std::make_unique<step::SeeCard>(*this, _playerId, true);
        break;
    case StepId::SeeSomeonesCard:
        _step = std::make_unique<step::SeeCard>(*this, _playerId, false);
        break;
    case StepId::SwapCardBlindly:
        _step = std::make_unique<step::SwapCard>(*this, _playerId, false);
        break;
    case StepId::SwapCardOpenly:
        _step = std::make_unique<step::SwapCard>(*this, _playerId, true);
        break;
    case StepId::TakeCard:
        _step = std::make_unique<step::TakeCard>(*this, _playerId);
        break;
    default:
        break;
    }
    CN_LOG_FRM("Set step {}, player {}", (unsigned)_nextStepId, (unsigned)_playerId);
}

Card* Board::getNextCard()
{
    CN_ASSERT(!m_cards.empty());
    Card* card = m_cards.back();
    m_cards.pop_back();

    return card;
}

void Board::discardCard(Card* _card)
{
    ++m_numberOfDiscardCards;
}

} // namespace cn::client::game
