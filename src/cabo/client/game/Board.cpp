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

Board::Board(const core::Context& _context, std::vector<game::Participant*>&& _participants, Deck& _deck, 
             menu::item::NotificationQueue& _queue, menu::item::Button& _finishButton, DecideActionButtons&& _decideActionButtons,
             DecideSwapButtons&& _decideSwapButtons, CardPositions _cardPositions)
    : m_contextRef(_context)
    , m_participants(std::move(_participants))
    , m_deckRef(_deck)
    , m_queueRef(_queue)
    , m_finishButtonRef(_finishButton)
    , m_decideActionButtons(std::move(_decideActionButtons))
    , m_decideSwapButtons(std::move(_decideSwapButtons))
    , m_cardPositions(_cardPositions)
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
        _dispatcher.registerEvent<events::LocalPlayerClickDeckEvent>(m_listenerId,
            [this](const events::LocalPlayerClickDeckEvent& _event)
            {
                auto it = m_steps.find(m_localPlayerId);
                if (it != m_steps.end() && it->second)
                    it->second->processPlayerInput(InputType::ClickPile, PileType::Deck);
            }
        );
        _dispatcher.registerEvent<events::LocalPlayerClickDiscardEvent>(m_listenerId,
            [this](const events::LocalPlayerClickDiscardEvent& _event)
            {
                auto it = m_steps.find(m_localPlayerId);
                if (it != m_steps.end())
                    it->second->processPlayerInput(InputType::ClickPile, PileType::Discard);
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
        _dispatcher.registerEvent<events::LocalPlayerClickDecideButtonEvent>(m_listenerId,
            [this](const events::LocalPlayerClickDecideButtonEvent& _event)
            {
                auto it = m_steps.find(m_localPlayerId);
                if (it != m_steps.end())
                    it->second->processPlayerInput(InputType::Action, _event.m_button);
            }
        );
        _dispatcher.registerEvent<events::LocalPlayerClickDecideSwapButtonEvent>(m_listenerId,
            [this](const events::LocalPlayerClickDecideSwapButtonEvent& _event)
            {
                auto it = m_steps.find(m_localPlayerId);
                if (it != m_steps.end())
                    it->second->processPlayerInput(InputType::SwapDecision, _event.m_swap);
            }
        );

        _dispatcher.registerEvent<events::LocalPlayerClickFinishButtonEvent>(m_listenerId,
            [this](const events::LocalPlayerClickFinishButtonEvent& _event)
            {
                auto it = m_steps.find(m_localPlayerId);
                if (it != m_steps.end())
                    it->second->processPlayerInput(InputType::Finish, std::monostate());
            }
        );
        _dispatcher.registerEvent<events::ProvideCardNetEvent>(m_listenerId,
            [&_dispatcher, this](const events::ProvideCardNetEvent& _event){
                bool found = false;
                for (auto* participant : m_participants)
                {
                    participant->visitSlots(
                        [&_event, &found](ParticipantSlot& _slot) {
                            if (_slot.cardPtr->getId() == _event.m_cardId)
                            {
                                _slot.cardPtr->set(_event.m_rank, _event.m_suit);
                                found = true;
                                return;
                            }
                        }
                    );
                }

                if (found)
                    return;
                if (m_drawnCardPtr && m_drawnCardPtr->getId() == _event.m_cardId)
                {
                    m_drawnCardPtr->set(_event.m_rank, _event.m_suit);
                    return;
                }
                
                for (auto* card : m_cardsToDiscard)
                {
                    if (card->getId() == _event.m_cardId)
                        card->set(_event.m_rank, _event.m_suit);
                }
            }
        );
        _dispatcher.registerEvent<events::PlayerSlotUpdateNetEvent>(m_listenerId,
            [&_dispatcher, this](const events::PlayerSlotUpdateNetEvent& _event){
                auto* participant = getParticipant(_event.m_playerId);
                if (_event.m_wasAdded)
                {
                    participant->addSlot(_event.m_slotId);
                    auto& slot = participant->getSlot(_event.m_slotId);
                    Card* card = getNextCardFromDeck();
                    slot.cardPtr = card;
                    slot.cardPtr->changeState({ 
                        .desiredPosition = slot.position,
                        .desiredRotation = slot.rotation,
                        .desiredState = Card::State::InHand,
                        .onFinishCallback = [&slot](){
                            slot.buttonRef.requestActivated();
                        }
                    });
                   
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
        _dispatcher.unregisterEvent<events::LocalPlayerClickDeckEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::LocalPlayerClickDiscardEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::LocalPlayerClickSlotEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::LocalPlayerClickDecideButtonEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::LocalPlayerClickDecideSwapButtonEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::LocalPlayerClickFinishButtonEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::ProvideCardNetEvent>(m_listenerId);
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
                        Card* card = getNextCardFromDeck();
                         _slot.cardPtr = card;
                        _slot.cardPtr->changeState({ 
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

void Board::showDecideActionButtons()
{
    for (auto& button : m_decideActionButtons)
        button->requestActivated();
}

void Board::hideDecideActionButtons()
{
    for (auto& button : m_decideActionButtons)
        button->requestDeactivated();
}

void Board::showDecideSwapButtons()
{
    for (auto& button : m_decideSwapButtons)
        button->requestActivated();
}

void Board::hideDecideSwapButtons()
{
    for (auto& button : m_decideSwapButtons)
        button->requestDeactivated();
}

void Board::showFinishButton()
{
    m_finishButtonRef.requestActivated();
}

void Board::hideFinishButton()
{
    m_finishButtonRef.requestDeactivated();
}

Card* Board::drawCard(bool _fromDeck)
{
    if (_fromDeck)
        m_drawnCardPtr = getNextCardFromDeck();
    else
        m_drawnCardPtr = getLastCardFromDiscard();

    return m_drawnCardPtr;
}

void Board::preDiscardCard(Card* _card)
{
    m_cardsToDiscard.push_back(_card);
}

void Board::discardCard(Card* _card)
{
    m_cardsToDiscard.erase(
        std::remove_if(m_cardsToDiscard.begin(), m_cardsToDiscard.end(),
            [_card](Card* _preCard){ return _preCard->getId() == _card->getId(); }),
        m_cardsToDiscard.end()
    );

    if (!m_discard.empty() && m_discard.back()->isActivated())
    {
        m_discard.back()->hide();
        m_discard.back()->requestDeactivated();
    }
    _card->show(true);
    m_discard.push_back(_card);
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
        _step = std::make_unique<step::MatchCard>(*this, _playerId, *m_drawnCardPtr);
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
        _step = std::make_unique<step::TakeCard>(*this, _playerId, *m_drawnCardPtr);
        break;
    default:
        break;
    }
    CN_LOG_FRM("Set step {}, player {}", (unsigned)_nextStepId, (unsigned)_playerId);
}

Card* Board::getNextCardFromDeck()
{
    return static_cast<Card*>(m_deckRef.getNextCard());
}

Card* Board::getLastCardFromDiscard()
{
    CN_ASSERT(!m_discard.empty());
    Card* card = m_discard.back();
    m_discard.pop_back();
    
    return card;
}

} // namespace cn::client::game
