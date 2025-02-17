#include "server/game/Board.hpp"
#include "server/game/step/SeeOwnCard.hpp"

#include "shared/events/NetworkEvents.hpp"
#include "shared/net/Manager.hpp"

#include "core/event/Dispatcher.hpp"
#include "core/Log.hpp"

#include <algorithm>

namespace cn::server::game
{

Board::Board(const core::Context& _context, Deck& _deck, std::vector<Participant*>&& _participants)
    : m_contextRef(_context)
    , m_deckRef(_deck)
{
    for (auto* p : _participants)
        m_participants.emplace_back(BoardParticipant{ .participantRef = *p, .currentStep = {} });

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

        // On Enter
        switch (m_state)
        {
        case BoardState::Start:
            break;
        case BoardState::Peek:
            for (auto& participant : m_participants)
                setParticipantStep(StepId::SeeOwnCard, participant);
            break;
        case BoardState::Game:
            break;
        case BoardState::Cabo:
            break;
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

void Board::setParticipantStep(StepId _stepId, BoardParticipant& _participantRef)
{   
    CN_ASSERT(_participantRef.currentStepId != _stepId);

    PlayerId playerId = _participantRef.participantRef.getId();
    _participantRef.currentStepId = _stepId;

    switch (_stepId)
    {
    case StepId::SeeOwnCard:
        _participantRef.currentStep = std::make_unique<step::SeeOwnCard>(*this, playerId);
        _participantRef.currentStep->registerEvents(getContext().get<core::event::Dispatcher>(), true);
        break;
    default:
        break;
    }
    CN_LOG_FRM("Set step {} to participant {}", (unsigned)_stepId, playerId);

    auto& netManRef = getContext().get<net::Manager>();
    events::PlayerStepUpdateEvent event(playerId, _stepId);
    netManRef.send(event);
}

} // namespace cn::server::game
