#include "server/game/Board.hpp"
#include "server/game/step/DrawCard.hpp"
#include "server/game/step/SeeOwnCard.hpp"

#include "shared/game/Board.hpp"
#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"

#include "core/event/Dispatcher.hpp"
#include "core/Log.hpp"

#include <algorithm>

namespace cn::server::game
{

Board::Board(const core::Context& _context, Deck& _deck, Discard& _discard, std::vector<Participant*>/**/&& _participants, PlayerId _firstParticipantTurn)
    : m_contextRef(_context)
    , m_deckRef(_deck)
    , m_discardRef(_discard)
{
    for (auto* p : _participants)
        m_participants.emplace_back(BoardParticipant{ .participantRef = *p });

    size_t index = 0;
    const size_t lastIndex = m_participants.size() - 1;
    for (const auto& p : m_participants)
    {
        if (p.participantRef.getId() == _firstParticipantTurn)
        {
            m_indexOfCurrentParticipantTurn = index == 0 ? lastIndex : (index == lastIndex ? 0 : index - 1);
            break;
        }
        ++index;
    }

    m_listenerId = core::event::getNewListenerId();
    m_newStateRequested = true;
}

Board::~Board() = default;

void Board::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::RemotePlayerInputNetEvent>(m_listenerId,
            [this] (const events::RemotePlayerInputNetEvent& _event){
                CN_LOG_FRM("Input {} from {} received", (unsigned)_event.m_inputType, (unsigned)_event.m_playerId);
                m_inputBuffer.emplace(_event.m_sentTimeRttBased.asMilliseconds(), _event);
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::PlayerTurnUpdateNetEvent>(m_listenerId);
    }
}

void Board::update(sf::Time _dt)
{
    if (m_newStateRequested)
    {
        m_newStateRequested = false;
        {
            BoardState newState;
            switch (m_state)
            {
                case BoardState::Start:
                newState = BoardState::Peek;
                break;
                case BoardState::Peek:
                newState = BoardState::Game;
                break;
                case BoardState::Game:
                newState = BoardState::Cabo;
                break;
                case BoardState::Cabo:
                newState = BoardState::Cabo;//?finish
                break;
            }
            m_state = newState;
        }
        CN_LOG_FRM("Board state {}", (int)m_state);

        auto& netManRef = getContext().get<net::Manager>();
        events::BoardStateUpdateNetEvent event(m_state);
        netManRef.send(event);

        // On Enter
        switch (m_state)
        {
        case BoardState::Start:
            break;
        case BoardState::Peek:
            for (auto& participant : m_participants)
                participantStartsTurn(participant);
            break;
        case BoardState::Game:
            break;
        case BoardState::Cabo:
            break;
        }
    }
    if (m_state == BoardState::Peek)
    {
        bool allParticipantFinished = true;
        for (auto& participant : m_participants)
        {
            if (participant.currentStepId != StepId::Idle)
                allParticipantFinished = false;
        }
        if (allParticipantFinished)
        {
            m_newStateRequested = true;
            m_currentParticipantFinishedTurn = true;
        }
    }
    else if (m_state == BoardState::Game || m_state == BoardState::Cabo)
    {
        if (m_participants.at(m_indexOfCurrentParticipantTurn).currentStepId == StepId::Idle)
            m_currentParticipantFinishedTurn = true;

        if (m_currentParticipantFinishedTurn)
        {
            m_currentParticipantFinishedTurn = false;

            m_indexOfCurrentParticipantTurn = getIndexOfNextParticipantTurn(m_indexOfCurrentParticipantTurn);
            participantStartsTurn(m_participants.at(m_indexOfCurrentParticipantTurn));
        }
    }

    constexpr int32_t TimeInInputBufferMs = 100; // TODO configure this or add to constants 
    
    for (auto it = m_inputBuffer.cbegin(); it != m_inputBuffer.cend();)
    {
        auto& systemClockRef = m_contextRef.get<sf::Clock>();
        int32_t currentTime = systemClockRef.getElapsedTime().asMilliseconds();
        if (currentTime - m_inputBuffer.begin()->first >= TimeInInputBufferMs)
        {
            processInputEvent(it->second);
            m_inputBuffer.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

Participant* Board::getParticipant(PlayerId _id)
{
    return &(getBoardParticipant(_id).participantRef);
}

Board::BoardParticipant& Board::getBoardParticipant(PlayerId _id)
{
    auto it = std::find_if(m_participants.begin(), m_participants.end(), 
        [_id](const BoardParticipant& _par){
            return _par.participantRef.getId() == _id;
        }
    );
    CN_ASSERT(it != m_participants.end());
    return (*it);
}

size_t Board::getIndexOfNextParticipantTurn(size_t _currentIndex) const
{
    _currentIndex++;
    if (_currentIndex >= m_participants.size())
        _currentIndex = 0;
    return _currentIndex;
}

void Board::processInputEvent(const events::RemotePlayerInputNetEvent& _event)
{
    auto& participant = getBoardParticipant(_event.m_senderPeerId);
   /*/*/ auto& netManRef = getContext().get<net::Manager>(); // ?from outside
    bool forwardEvent = true;

    switch (m_state)
    {
    case BoardState::Peek:
        if (participant.currentStepId == StepId::SeeOwnCard)
        {
            processSeeCardStep(_event, participant);
        }
        else if (participant.currentStepId == StepId::FinishTurn)
        {
            if (_event.m_inputType != InputType::Finish)
            {
                CN_ASSERT(false);
                break;
            }
            CN_ASSERT(_event.m_senderPeerId == _event.m_playerId);
            CN_LOG_FRM("Participant {} finishes turn.", _event.m_senderPeerId);
            participant.currentStepId = StepId::Idle;
        }
        else
        {
            CN_LOG_FRM("Cannot process the event in the current step: participant {}, step: {}", _event.m_senderPeerId, (int)participant.currentStepId);
            // TODO ?punish
        } 
        break;
    case BoardState::Game:
        if (participant.currentStepId == StepId::DecideCard)
        {
            if (_event.m_inputType != InputType::DecideButton)
            {
                CN_ASSERT(false);
                // TODO punish?
                break;
            }
            DecideButton button = std::get<DecideButton>(_event.m_data);
            participant.currentStepId = shared::game::getNextStepId(button, Card::getAbility(Card(m_drawnCardPtr->getRank(), m_drawnCardPtr->getSuit())));
            
            CN_LOG_FRM("Decide button, participant: {}, button: {}", _event.m_senderPeerId, (int)button);
            
            if (button == DecideButton::Discard)
            {
                events::DiscardCardNetEvent eventDiscard(m_drawnCardPtr->getRank(), m_drawnCardPtr->getSuit());
                netManRef.send(eventDiscard);
                CN_LOG_FRM("Discard card, participant: {}, card: {} {}", _event.m_senderPeerId, (int)m_drawnCardPtr->getRank(), (int)m_drawnCardPtr->getSuit());
            }
        }
        else if (participant.currentStepId == StepId::DrawCard)
        {
            if (_event.m_inputType != InputType::ClickPile)
            {
                CN_ASSERT(false);
                // TODO punish?
                break;
            }
            PileType pileType = std::get<PileType>(_event.m_data);
            Card* card = nullptr;
            if (pileType == PileType::Deck)
            {
                card = m_deckRef.getNextCard();
                m_drawnCardPtr = card;
                participant.currentStepId = StepId::DecideCard;
                events::ProvideCardNetEvent event(card->getRank(), card->getSuit());
                netManRef.send(event, _event.m_senderPeerId); 
            }
            else
            {
                // m_discardRef
                CN_ASSERT(false);
                participant.currentStepId = StepId::TakeCard; // Idle?
            }

            CN_LOG_FRM("Draw a card, participant: {}, pile: {}, card: {} {}", 
                _event.m_senderPeerId, (int)pileType, (int)card->getRank(), (int)card->getSuit()
            );
        }
        else if (participant.currentStepId == StepId::FinishTurn)
        {
            if (_event.m_inputType != InputType::Finish)
            {
                CN_ASSERT(false);
                break;
            }
            CN_ASSERT(_event.m_senderPeerId == _event.m_playerId);
            participant.currentStepId = StepId::Idle;
            CN_LOG_FRM("Participant {} finishes turn.", _event.m_senderPeerId);
        }
        else if (participant.currentStepId == StepId::MatchCard)
        {
            processMatchCardStep(_event, participant);
        }
        else if (participant.currentStepId == StepId::SeeOwnCard || participant.currentStepId == StepId::SeeSomeonesCard)
        {
            processSeeCardStep(_event, participant);
        }
        else if (participant.currentStepId == StepId::TakeCard)
        {
            if (_event.m_inputType != InputType::ClickSlot)
            {
                CN_ASSERT(false);
                // TODO punish?
                break;
            }

            ClickSlotInputData dataStruct = std::get<ClickSlotInputData>(_event.m_data);
            auto* card = participant.participantRef.replace(dataStruct.slotId, m_drawnCardPtr);
            m_discardRef.discard(card);

            events::DiscardCardNetEvent event(card->getRank(), card->getSuit());
            netManRef.send(event); 

            participant.currentStepId = StepId::FinishTurn;

            CN_LOG_FRM("Take card, participant: {}, slot: {}, drawn card: {} {}, discarded card: {} {}", 
                _event.m_senderPeerId, dataStruct.slotId, (int)m_drawnCardPtr->getRank(), (int)m_drawnCardPtr->getSuit(),
                (int)card->getRank(), (int)card->getSuit() 
            );
        }
        else
        {
            CN_ASSERT(false);
            // TODO ?punish
        } 
    break;
    case BoardState::Cabo:
        CN_ASSERT(false);
        break;
    default:
        break;
    }

    if (forwardEvent)
        netManRef.send(_event, nsf::MessageInfo::Type::EXCLUDE_BRODCAST, true, _event.m_senderPeerId);
}

void Board::processMatchCardStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant)
{
    if (_event.m_inputType != InputType::ClickSlot)
    {
        CN_ASSERT(false);
        // TODO punish?
        return;
    }
    auto& netManRef = getContext().get<net::Manager>(); 

    ClickSlotInputData dataStruct = std::get<ClickSlotInputData>(_event.m_data);
    auto* card = _participant.participantRef.getCard(dataStruct.slotId);

    bool success = card->getRank() == m_drawnCardPtr->getRank();

    m_discardRef.discard(m_drawnCardPtr);
    events::DiscardCardNetEvent event(m_drawnCardPtr->getRank(), m_drawnCardPtr->getSuit());
    netManRef.send(event); 
    
    events::MatchCardNetEvent eventMatch(card->getRank(), card->getSuit(), success);
    netManRef.send(eventMatch); 

    ParticipantSlotId updatedSlotId = shared::game::ParticipantSlotIdInvalid;
    if (success)
    {
        // Everyone will discard it locally
        m_discardRef.discard(card);
        updatedSlotId = dataStruct.slotId;
        _participant.participantRef.removeSlot(dataStruct.slotId);
    }
    else
    {
        updatedSlotId = _participant.participantRef.addSlot();
        if (updatedSlotId != shared::game::ParticipantSlotIdInvalid)
            _participant.participantRef.replace(updatedSlotId, m_deckRef.getNextCard());
    }
    // if slot id is invalid, it means no available space left
    if (updatedSlotId != shared::game::ParticipantSlotIdInvalid)
    {
        events::PlayerSlotUpdateNetEvent event(_event.m_senderPeerId, updatedSlotId, !success);
        netManRef.send(event);
    }

    _participant.currentStepId = StepId::FinishTurn;

    CN_LOG_FRM("Match card, participant: {}, slot: {}, new slot: {}, drawn card: {} {}, matched card: {} {}", 
        _event.m_senderPeerId, dataStruct.slotId, updatedSlotId, (int)m_drawnCardPtr->getRank(), (int)m_drawnCardPtr->getSuit(),
        (int)card->getRank(), (int)card->getSuit() 
    );
}

void Board::processSeeCardStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant)
{
    if (_event.m_inputType != InputType::ClickSlot)
    {
        CN_ASSERT(false);
        // TODO punish?
        return;
    }
    ClickSlotInputData dataStruct = std::get<ClickSlotInputData>(_event.m_data);
    auto& slotOwner = getBoardParticipant(dataStruct.playerId);
    
    auto* card = slotOwner.participantRef.getCard(dataStruct.slotId);
    
    events::ProvideCardNetEvent event(card->getRank(), card->getSuit());
    getContext().get<net::Manager>().send(event, _event.m_senderPeerId);            
    
    _participant.currentStepId = StepId::FinishTurn;

    CN_LOG_FRM("See card, participant: {}, slotOwner: {}, slot: {}, card: {} {}", 
        _event.m_senderPeerId, dataStruct.playerId, dataStruct.slotId, (int)card->getRank(), (int)card->getSuit()
    );
}

void Board::participantStartsTurn(BoardParticipant& _participant)
{
    PlayerId playerId = _participant.participantRef.getId();
    CN_LOG_FRM("Participant {} starts turn", playerId);

    auto& netManRef = getContext().get<net::Manager>();
    events::PlayerTurnUpdateNetEvent event(playerId, true);
    netManRef.send(event);

    StepId nextStepId = shared::game::getFirstStepId(m_state);
    setParticipantStep(nextStepId, _participant);
}

void Board::setParticipantStep(StepId _stepId, BoardParticipant& _participant)
{
    CN_ASSERT(_participant.currentStepId != _stepId);

    PlayerId playerId = _participant.participantRef.getId();
    _participant.currentStepId = _stepId;

    CN_LOG_FRM("Set step {} to participant {}", (unsigned)_stepId, playerId);
}

} // namespace cn::server::game
