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

Board::Board(const core::Context& _context, Deck& _deck, std::vector<Participant*>/**/&& _participants, PlayerId _firstParticipantTurn)
    : m_contextRef(_context)
    , m_deckRef(_deck)
{
    for (auto* p : _participants)
        m_participants.emplace_back(BoardParticipant{ .participantRef = *p, .currentStep = {} });

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
    }
    else
    {
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
            if (!participant.currentStep->isFinished())
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
        if (m_currentParticipantFinishedTurn)
        {
            m_currentParticipantFinishedTurn = false;

            m_indexOfCurrentParticipantTurn = getIndexOfNextParticipantTurn(m_indexOfCurrentParticipantTurn);
            participantStartsTurn(m_participants.at(m_indexOfCurrentParticipantTurn));
        }
    }
    for (auto& participant : m_participants)
    {
        if (participant.currentStep)
            participant.currentStep->update(_dt);
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

    auto& eventDispatcherRef = getContext().get<core::event::Dispatcher>();
    if (_participant.currentStep)
        _participant.currentStep->registerEvents(eventDispatcherRef, false);

    switch (_stepId)
    {
    case StepId::Idle:
        _participant.currentStep.reset();
        break;
    case StepId::SeeOwnCard:
        _participant.currentStep = std::make_unique<step::SeeOwnCard>(*this, playerId);
        break;
    case StepId::DrawCard:
        _participant.currentStep = std::make_unique<step::DrawCard>(*this, playerId);
        break;
    default:
        break;
    }
    if (_participant.currentStep)
        _participant.currentStep->registerEvents(eventDispatcherRef, true);

    CN_LOG_FRM("Set step {} to participant {}", (unsigned)_stepId, playerId);
}

} // namespace cn::server::game
