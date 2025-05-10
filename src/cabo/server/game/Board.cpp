#include "server/game/Board.hpp"

#include "shared/game/Board.hpp"
#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"

#include "core/event/Dispatcher.hpp"
#include "core/Log.hpp"

#include <algorithm>

namespace
{
    // Add an offset on the server 
    inline constexpr short unsigned TimeForPlayerTurnServerS = cn::shared::game::TimeForPlayerTurnS + 2;
} // namespace

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
    m_state = BoardState::Finish;
    m_newStateRequested = true;
    m_timeoutDt = sf::seconds(shared::game::TimeBeforeStartS);
}

Board::~Board() = default;

void Board::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::RemotePlayerInputNetEvent>(m_listenerId,
            [this] (const events::RemotePlayerInputNetEvent& _event){
                CN_LOG_FRM("Input {} from {} received", (unsigned)_event.m_inputType, _event.m_playerId.value());
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
        m_state = getNextState(m_state);

        CN_LOG_FRM("Board state {}", (int)m_state);

        auto& netManRef = getContext().get<net::Manager>();
        events::BoardStateUpdateNetEvent event(m_state);
        netManRef.send(event);

        enterState(m_state);
    }

    updateState(m_state, _dt);

    processInputs();
}

BoardState Board::getNextState(BoardState _state) const
{
    BoardState newState;
    switch (_state)
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
        newState = BoardState::Finish;
        break;
    case BoardState::Finish:
        newState = BoardState::Start;
        break;
    }
    return newState;
}

void Board::enterState(BoardState _state)
{
    switch (_state)
    {
    case BoardState::Start:
    
        for (auto& participant : m_participants)
        {
            participant.participantRef.visitSlots(
                [this](ParticipantSlot& _slot) {
                    Card* card = getNextCardFromDeck();
                    _slot.card = card;
                }
            );
        }
        break;
    case BoardState::Peek:
        m_timeoutDt = sf::seconds(TimeForPlayerTurnServerS);
        for (auto& participant : m_participants)
            participantStartsTurn(participant);
        break;
    case BoardState::Game:
        break;
    case BoardState::Cabo:
        m_playersLeftBeforeEnd = m_participants.size();
        break;
    case BoardState::Finish:
        for (auto& participant : m_participants)
            participantStartsTurn(participant);
        for (auto& participant : m_participants)
        {
            participant.participantRef.visitSlots(
                [this](ParticipantSlot& _slot) {
                    events::ProvideCardNetEvent event(_slot.card->getRank(), _slot.card->getSuit(), _slot.card->getId());
                    getContext().get<net::Manager>().send(event);
                    CN_LOG_FRM("Card id: {}, {} {}", _slot.card->getId().value(), (int)_slot.card->getRank(), (int)_slot.card->getSuit());
                }
            );
            participant.currentStepId = StepId::FinishTurn;
        }
        break;
    }
}

void Board::updateState(BoardState _state, sf::Time _dt)
{
    switch (_state)
    {
    case BoardState::Start:
        m_timeoutDt -= _dt;
        if (m_timeoutDt.asSeconds() <= 0.f)
        {
            m_newStateRequested = true;
        }
        break;
    case BoardState::Peek:
    {
        bool allParticipantFinished = true;
        for (auto& participant : m_participants)
        {
            if (participant.currentStepId != StepId::Idle)
            allParticipantFinished = false;
        }
        
        m_timeoutDt -= _dt;
        if (m_timeoutDt.asSeconds() <= 0.f)
        {
            allParticipantFinished = true;
            for (auto& participant : m_participants)
            {
                if (participant.currentStepId != StepId::Idle)
                onParticipantTimeout(participant);
            }
        }
        if (allParticipantFinished)
        {
            m_newStateRequested = true;
            m_currentParticipantFinishedTurn = true;
        }
        break;
    }
    case BoardState::Game:
        if (m_participants.at(m_indexOfCurrentParticipantTurn).currentStepId == StepId::Idle)
            m_currentParticipantFinishedTurn = true;

        m_timeoutDt -= _dt;
        if (m_timeoutDt.asSeconds() <= 0.f)
        {
            onParticipantTimeout(m_participants.at(m_indexOfCurrentParticipantTurn));
            m_currentParticipantFinishedTurn = true;
        }

        if (m_currentParticipantFinishedTurn)
        {
            m_currentParticipantFinishedTurn = false;

            m_indexOfCurrentParticipantTurn = getIndexOfNextParticipantTurn(m_indexOfCurrentParticipantTurn);
            participantStartsTurn(m_participants.at(m_indexOfCurrentParticipantTurn));
            m_timeoutDt = sf::seconds(TimeForPlayerTurnServerS);
        }
        break;
    case BoardState::Cabo:
        if (m_participants.at(m_indexOfCurrentParticipantTurn).currentStepId == StepId::Idle)
            m_currentParticipantFinishedTurn = true;

        m_timeoutDt -= _dt;
        if (m_timeoutDt.asSeconds() <= 0.f)
        {
            onParticipantTimeout(m_participants.at(m_indexOfCurrentParticipantTurn));
            m_currentParticipantFinishedTurn = true;
        }

        if (m_currentParticipantFinishedTurn)
        {
            m_currentParticipantFinishedTurn = false;

            m_indexOfCurrentParticipantTurn = getIndexOfNextParticipantTurn(m_indexOfCurrentParticipantTurn);
            participantStartsTurn(m_participants.at(m_indexOfCurrentParticipantTurn));
            m_timeoutDt = sf::seconds(TimeForPlayerTurnServerS);

            --m_playersLeftBeforeEnd;
            if(m_playersLeftBeforeEnd == 0)
                m_newStateRequested = true;    
        }
        break;
    case BoardState::Finish:
    // TODO copypasted code
        bool allParticipantFinished = true;
        for (auto& participant : m_participants)
        {
            if (participant.currentStepId != StepId::Idle)
            allParticipantFinished = false;
        }
        
        m_timeoutDt -= _dt;
        if (m_timeoutDt.asSeconds() <= 0.f)
        {
            allParticipantFinished = true;
            for (auto& participant : m_participants)
            {
                if (participant.currentStepId != StepId::Idle)
                onParticipantTimeout(participant);
            }
        }
        if (allParticipantFinished)
        {
            m_isFinished = true;
            m_currentParticipantFinishedTurn = true;
        }
        break;
    }
}

void Board::processInputs()
{
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
    auto& participant = getBoardParticipant(_event.m_playerId);
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
            processFinishTurnStep(_event, participant);
        }
        else
        {
            CN_LOG_FRM("Cannot process the event in the current step: participant {}, step: {}", _event.m_senderPeerId, (int)participant.currentStepId);
            // TODO ?punish
        } 
        break;
    case BoardState::Game:
    case BoardState::Cabo:
        if (participant.currentStepId == StepId::DecideCard)
        {
            processDecideCardStep(_event, participant);
        }
        else if (participant.currentStepId == StepId::DrawCard)
        {
            processDrawCardStep(_event, participant);
        }
        else if (participant.currentStepId == StepId::FinishTurn)
        {
            processFinishTurnStep(_event, participant);
        }
        else if (participant.currentStepId == StepId::MatchCard)
        {
            processMatchCardStep(_event, participant);
        }
        else if (participant.currentStepId == StepId::SeeOwnCard || participant.currentStepId == StepId::SeeSomeonesCard)
        {
            processSeeCardStep(_event, participant);
        }
        else if (participant.currentStepId == StepId::SwapCardBlindly || participant.currentStepId == StepId::SwapCardOpenly)
        {
            processSwapCardStep(_event, participant);
        }
        else if (participant.currentStepId == StepId::TakeCard)
        {
            processTakeCardStep(_event, participant);
        }
        else
        {
            CN_ASSERT(false);
            // TODO ?punish
        } 
        break;
    case BoardState::Finish:
        if (participant.currentStepId == StepId::FinishTurn)
        {
            processFinishTurnStep(_event, participant);
        }
        else
        {
            CN_LOG_FRM("Cannot process the event in the current step: participant {}, step: {}", _event.m_senderPeerId, (int)participant.currentStepId);
        } 
        break;
    default:
        break;
    }

    if (forwardEvent)
        getContext().get<net::Manager>().send(_event, nsf::MessageInfo::Type::EXCLUDE_BRODCAST, true, _event.m_senderPeerId);
}

void Board::processDecideCardStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant)
{
    if (_event.m_inputType != InputType::Action)
    {
        CN_ASSERT(false);
        // TODO punish?
        return;
    }
    ActionType button = std::get<ActionType>(_event.m_data);
    _participant.currentStepId = shared::game::getNextStepId(button, Card::getAbility(Card(m_drawnCardPtr->getRank(), m_drawnCardPtr->getSuit())));
    
    CN_LOG_FRM("Decide button, participant: {}, button: {}", _event.m_senderPeerId, (int)button);
    
    if (button == ActionType::Discard || button == ActionType::Ability)
    {
        discardCard(m_drawnCardPtr);
        CN_LOG_FRM("Discard card, participant: {}, card: {} {}", _event.m_senderPeerId, (int)m_drawnCardPtr->getRank(), (int)m_drawnCardPtr->getSuit());
    }
}

void Board::processDrawCardStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant)
{
    if (_event.m_inputType != InputType::ClickPile)
    {
        CN_ASSERT(false);
        // TODO punish?
        return;
    }
    PileType pileType = std::get<PileType>(_event.m_data);
    Card* card = nullptr;
    if (pileType == PileType::Deck)
    {
        card = getNextCardFromDeck();
        m_drawnCardPtr = card;
        _participant.currentStepId = StepId::DecideCard;
        events::ProvideCardNetEvent event(card->getRank(), card->getSuit(), card->getId());
        getContext().get<net::Manager>().send(event, _event.m_senderPeerId); 
    }
    else
    {
        card = m_discardRef.getLast();
        m_drawnCardPtr = card;
        _participant.currentStepId = StepId::TakeCard;
    }

    CN_LOG_FRM("Draw a card, participant: {}, pile: {}, card: {} {}", 
        _event.m_senderPeerId, (int)pileType, (int)card->getRank(), (int)card->getSuit()
    );
}

void Board::processFinishTurnStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant)
{
    if (_event.m_inputType == InputType::Finish)
    {
        CN_ASSERT(_event.m_senderPeerId == _event.m_playerId.value());
        _participant.currentStepId = StepId::Idle;
        CN_LOG_FRM("Participant {} finishes turn.", _event.m_senderPeerId);
    }
    else if (_event.m_inputType == InputType::Cabo)
    {
        CN_ASSERT(_event.m_senderPeerId == _event.m_playerId.value());

        m_newStateRequested = true;
        _participant.currentStepId = StepId::Idle;
        CN_LOG_FRM("Participant {} says Cabo.", _event.m_senderPeerId);
    }
    else
    {
        CN_ASSERT(false);
    }
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

    discardCard(m_drawnCardPtr);

    events::ProvideCardNetEvent eventDiscard(card->getRank(), card->getSuit(), card->getId());
    getContext().get<net::Manager>().send(eventDiscard);

    ParticipantSlotId updatedSlotId{};
    if (success)
    {
        discardCard(card);
        updatedSlotId = dataStruct.slotId;
        _participant.participantRef.removeSlot(dataStruct.slotId);
    }
    else
    {
        updatedSlotId = _participant.participantRef.addSlot();
        if (updatedSlotId.isValid())
            _participant.participantRef.replace(updatedSlotId, getNextCardFromDeck());
    }
    // if slot id is invalid, it means no available space left
    if (updatedSlotId.isValid())
    {
        events::PlayerSlotUpdateNetEvent event(_event.m_playerId, updatedSlotId, !success);
        netManRef.send(event);
    }

    _participant.currentStepId = StepId::FinishTurn;

    CN_LOG_FRM("Match card, participant: {}, slot: {}, new slot: {}, drawn card: {} {}, matched card: {} {}", 
        _event.m_senderPeerId, dataStruct.slotId.value(), updatedSlotId.value(), (int)m_drawnCardPtr->getRank(), (int)m_drawnCardPtr->getSuit(),
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
    
    events::ProvideCardNetEvent event(card->getRank(), card->getSuit(), card->getId());
    getContext().get<net::Manager>().send(event, _event.m_senderPeerId);            
    
    _participant.currentStepId = StepId::FinishTurn;

    CN_LOG_FRM("See card, participant: {}, slotOwner: {}, slot: {}, card: {} {}", 
        _event.m_senderPeerId, dataStruct.playerId.value(), dataStruct.slotId.value(), (int)card->getRank(), (int)card->getSuit()
    );
}

void Board::processSwapCardStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant)
{
    if (_event.m_inputType == InputType::ClickSlot)
    {
        if (m_isFirstPartOfSwap)
        {
            if (_participant.currentStepId == StepId::SwapCardOpenly)
            {
                processSeeCardStep(_event, _participant);
                _participant.currentStepId = StepId::SwapCardOpenly;

            }
            
            m_swapData = std::get<ClickSlotInputData>(_event.m_data);
            m_isFirstPartOfSwap = false;
        }
        else
        {
            CN_ASSERT(_participant.currentStepId == StepId::SwapCardBlindly);
            
            ClickSlotInputData ownData = std::get<ClickSlotInputData>(_event.m_data);
            CN_ASSERT(ownData.playerId.value() == _event.m_senderPeerId);

            auto& otherParticipant = getBoardParticipant(m_swapData.playerId);
    
            auto* ownCard = _participant.participantRef.getCard(ownData.slotId);
            auto* someonesCard = otherParticipant.participantRef.replace(m_swapData.slotId, ownCard);
            _participant.participantRef.replace(ownData.slotId, someonesCard);
            
            _participant.currentStepId = StepId::FinishTurn;
            m_isFirstPartOfSwap = true;

            CN_LOG_FRM("Swap card, participant: {}, slot: {}, card: {} {}, other participant: {}, slot: {}, card: {} {}", 
                _event.m_senderPeerId, ownData.slotId.value(), (int)ownCard->getRank(), (int)ownCard->getSuit(), 
                m_swapData.playerId.value(), m_swapData.slotId.value(), (int)someonesCard->getRank(), (int)someonesCard->getSuit() 
            );
        }
    }
    else if (_event.m_inputType == InputType::SwapDecision)
    {
        bool swapped = std::get<bool>(_event.m_data);
        if (swapped)
        {
            _participant.currentStepId = StepId::SwapCardBlindly;
        }
        else
        {
            m_isFirstPartOfSwap = true;
            _participant.currentStepId = StepId::FinishTurn;
        }
        CN_LOG_FRM("Swap decision, participant: {}, swapped: {}", _event.m_senderPeerId, swapped);
    }
    else
    {
        CN_ASSERT(false);
        // TODO punish?
    }
}

void Board::processTakeCardStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant)
{
    if (_event.m_inputType != InputType::ClickSlot)
    {
        CN_ASSERT(false);
        // TODO punish?
        return;
    }

    ClickSlotInputData dataStruct = std::get<ClickSlotInputData>(_event.m_data);
    auto* card = _participant.participantRef.replace(dataStruct.slotId, m_drawnCardPtr);
    
    m_discardRef.discard(card);
    events::ProvideCardNetEvent event(card->getRank(), card->getSuit(), card->getId());
    getContext().get<net::Manager>().send(event); 

    _participant.currentStepId = StepId::FinishTurn;

    CN_LOG_FRM("Take card, participant: {}, slot: {}, drawn card: {} {}, discarded card: {} {}", 
        _event.m_senderPeerId, dataStruct.slotId.value(), (int)m_drawnCardPtr->getRank(), (int)m_drawnCardPtr->getSuit(),
        (int)card->getRank(), (int)card->getSuit() 
    );
}

void Board::participantStartsTurn(BoardParticipant& _participant)
{
    PlayerId playerId = _participant.participantRef.getId();
    CN_LOG_FRM("Participant {} starts turn", playerId.value());

    auto& netManRef = getContext().get<net::Manager>();
    events::PlayerTurnUpdateNetEvent event(playerId, true);
    netManRef.send(event);

    StepId nextStepId = shared::game::getFirstStepId(m_state);
    setParticipantStep(nextStepId, _participant);
}

void Board::onParticipantTimeout(BoardParticipant& _participant)
{
    switch (_participant.currentStepId)
    {
    case StepId::DecideCard:
    {
        discardCard(m_drawnCardPtr);
        break;
    }
    case StepId::DiscardCard:
        break;
    case StepId::DrawCard:
        break;
    case StepId::FinishTurn:
        break;
    case StepId::Idle:
        break;
    case StepId::MatchCard:
        break;
    case StepId::SeeOwnCard:
        break;
    case StepId::SeeSomeonesCard:
        break;
    case StepId::SwapCardBlindly:
        m_isFirstPartOfSwap = true;
        break;
    case StepId::SwapCardOpenly:
        m_isFirstPartOfSwap = true;
        break;
    case StepId::TakeCard:
        break;
    }

    _participant.currentStepId = StepId::Idle;
    CN_LOG_FRM("Timeout, Player {} finished their turn", _participant.participantRef.getId().value());
}

void Board::setParticipantStep(StepId _stepId, BoardParticipant& _participant)
{
    CN_ASSERT(_participant.currentStepId != _stepId);

    PlayerId playerId = _participant.participantRef.getId();
    _participant.currentStepId = _stepId;

    CN_LOG_FRM("Set step {} to participant {}", (unsigned)_stepId, playerId.value());
}

void Board::discardCard(Card* _card)
{
    CN_LOG_FRM("Discard {}, {} {}", _card->getId().value(), (int)_card->getRank(), (int)_card->getSuit());

    events::ProvideCardNetEvent eventDiscard(_card->getRank(), _card->getSuit(), _card->getId());
    getContext().get<net::Manager>().send(eventDiscard);
    m_discardRef.discard(_card);
}

Card* Board::getNextCardFromDeck()
{
    return static_cast<Card*>(m_deckRef.getNextCard());
}

} // namespace cn::server::game
