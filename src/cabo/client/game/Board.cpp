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

Board::Board(const core::Context& _context, std::vector<game::Participant*>&& _participants, menu::item::NotificationQueue& _queue,
             menu::item::Button& _finishButton, menu::item::SimpleImage& _deckCardImage, menu::item::SimpleImage& _discardCardImage,
             menu::item::SimpleImage& _matchedCardImageRef, DecideActionButtons&& _decideActionButtons, DecideSwapButtons&& _decideSwapButtons)
    : m_contextRef(_context)
    , m_participants(std::move(_participants))
    , m_queueRef(_queue)
    , m_finishButtonRef(_finishButton)
    , m_deckCardImageRef(_deckCardImage)
    , m_discardCardImageRef(_discardCardImage)
    , m_matchedCardImageRef(_matchedCardImageRef)
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
                
                if (_event.m_playerId != m_localPlayerId)
                {
                    m_queueRef.push("Remote player starts turn");
                    // TODO show a notification
                    return;
                }
                m_queueRef.push("Your turn");
                
                CN_ASSERT(_event.m_hasTurnStarted); // TODO to implement the turn timeout
                
                StepId nextStepId = shared::game::getFirstStepId(m_boardState);
                CN_ASSERT(nextStepId != m_localPlayerStepId);
                
                changeStep(nextStepId);
            }
        );
        _dispatcher.registerEvent<events::DiscardCardNetEvent>(m_listenerId,
            [&_dispatcher, this](const events::DiscardCardNetEvent& _event){
                discardCard(Card(_event.m_rank, _event.m_suit));
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
        _dispatcher.unregisterEvent<events::DiscardCardNetEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::PlayerSlotUpdateNetEvent>(m_listenerId);
    }
}

void Board::update(sf::Time _dt)
{
    if (m_localPlayerStep)
    {
        m_localPlayerStep->update(_dt);

        if (m_localPlayerStep->isFinished())
        {
            StepId nextStepId = m_localPlayerStep->getNextStepId();
            CN_ASSERT(nextStepId != m_localPlayerStepId);

            changeStep(nextStepId);

            if (m_localPlayerStepId == StepId::Idle)
            {
                // there is no really need for the client to sent it
                // auto& netManRef = getContext().get<net::Manager>();
                // events::PlayerTurnUpdateNetEvent event(m_localPlayerId, false);
                // netManRef.send(event);
            }
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

void Board::onParticipantStartDeciding(Card _card)
{
    m_drawnCard = _card;
    m_deckCardImageRef.setTextureRect(game::spriteSheet::getCardTextureRect(_card.getRank(), _card.getSuit()));
    m_deckCardImageRef.requestActivated();
    for (auto& button : m_decideActionButtons)
        button->requestActivated();
}

void Board::onParticipantFinishDeciding()
{
    m_deckCardImageRef.requestDeactivated();
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

void Board::onShowMatchedCard(Card _card)
{
    m_matchedCardImageRef.setTextureRect(game::spriteSheet::getCardTextureRect(_card.getRank(), _card.getSuit()));
    m_matchedCardImageRef.requestActivated();
    m_matchedCard = _card;
}

void Board::onHideMatchedCard(bool _discard)
{
    m_matchedCardImageRef.requestDeactivated();
    if (_discard)
        discardCard(m_matchedCard);
}

void Board::changeStep(StepId _nextStepId)
{
    m_localPlayerStepId = _nextStepId;

    auto& eventDispatcherRef = getContext().get<core::event::Dispatcher>();
    if (m_localPlayerStep)
        m_localPlayerStep->registerEvents(eventDispatcherRef, false);

    switch (_nextStepId)
    {
    case StepId::Idle:
        m_localPlayerStep.reset();
        break;
    case StepId::DecideCard:
        m_queueRef.push("Decide the card");
        m_localPlayerStep = std::make_unique<step::DecideCard>(*this, m_localPlayerId);
        break;
    case StepId::DrawCard:
        m_queueRef.push("You can draw a card from the deck or the discard");
        m_localPlayerStep = std::make_unique<step::DrawCard>(*this, m_localPlayerId);
        break;
    case StepId::FinishTurn:
        m_queueRef.push("Finish or cabo");
        m_localPlayerStep = std::make_unique<step::Finish>(*this, m_localPlayerId);
        break;
    case StepId::MatchCard:
        m_queueRef.push("You can pick any of your card");
        m_localPlayerStep = std::make_unique<step::MatchCard>(*this, m_localPlayerId);
        break;
    case StepId::SeeOwnCard:
        m_queueRef.push("You can peek any of your card");
        m_localPlayerStep = std::make_unique<step::SeeCard>(*this, m_localPlayerId, true);
        break;
    case StepId::SeeSomeonesCard:
        m_queueRef.push("You can peek any card except yours");
        m_localPlayerStep = std::make_unique<step::SeeCard>(*this, m_localPlayerId, false);
        break;
    case StepId::SwapCardBlindly:
        m_queueRef.push("You can pick any card except yours");
        m_localPlayerStep = std::make_unique<step::SwapCard>(*this, m_localPlayerId, false);
        break;
    case StepId::SwapCardOpenly:
        m_queueRef.push("You can pick any card except yours");
        m_localPlayerStep = std::make_unique<step::SwapCard>(*this, m_localPlayerId, true);
        break;
    case StepId::TakeCard:
        m_queueRef.push("You can pick any of your card");
        m_localPlayerStep = std::make_unique<step::TakeCard>(*this, m_localPlayerId);
        break;
    default:
        break;
    }
    if (m_localPlayerStep)
        m_localPlayerStep->registerEvents(eventDispatcherRef, true);
    
    CN_LOG_FRM("Set step {}", (unsigned)_nextStepId);
}

void Board::discardCard(Card _card)
{
    m_discardCardImageRef.setTextureRect(game::spriteSheet::getCardTextureRect(_card.getRank(), _card.getSuit()));
    if (m_numberOfDiscardCards == 0)
        m_discardCardImageRef.requestActivated();
    ++m_numberOfDiscardCards;
}

} // namespace cn::client::game
