#pragma once

#include "server/game/Deck.hpp"
#include "server/game/Discard.hpp"
#include "server/game/Participant.hpp"
#include "server/game/Step.hpp"
#include "server/game/Types.hpp"

#include "core/event/Types.hpp"
#include "core/Context.hpp"

#include "shared/events/NetworkEvents.hpp"

#include <map>
#include <vector>

namespace cn::server::game
{
class Card;

class Board final
{
public:
    Board(const core::Context& _context, Deck& _deck, Discard& _discard, std::vector<Participant*>&& _participants, PlayerId _firstParticipantTurn);
    ~Board();

    Deck& getDeck() const { return m_deckRef; }

    const core::Context& getContext() const { return m_contextRef; };
    Participant* getParticipant(PlayerId _id);

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

    void update(sf::Time _dt);

private:
    struct BoardParticipant
    {
        Participant& participantRef;
        StepId currentStepId = StepId::Idle;
    };
    BoardParticipant& getBoardParticipant(PlayerId _id);
    void participantStartsTurn(BoardParticipant& _participant);
    void setParticipantStep(StepId _stepId, BoardParticipant& _participantRef);
    size_t getIndexOfNextParticipantTurn(size_t _currentIndex) const;
    void processInputEvent(const events::RemotePlayerInputNetEvent& _event);

    const core::Context& m_contextRef;
    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;

    Deck& m_deckRef;
    Discard& m_discardRef;
    std::vector<BoardParticipant> m_participants;

    size_t m_indexOfCurrentParticipantTurn = 0;
    bool m_currentParticipantFinishedTurn = false;

    BoardState m_state = BoardState::Start;
    bool m_newStateRequested = false;

    std::map<int32_t, events::RemotePlayerInputNetEvent> m_inputBuffer;

    Card* m_drawnCardPtr = nullptr;
};

} // namespace cn::server::game
