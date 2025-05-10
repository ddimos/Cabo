#pragma once

#include "server/game/Deck.hpp"
#include "server/game/Discard.hpp"
#include "server/game/Participant.hpp"
#include "server/game/Types.hpp"

#include "core/event/Types.hpp"
#include "core/Context.hpp"

#include "shared/events/NetworkEvents.hpp"

#include <SFML/System/Time.hpp>

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

    bool isFinished() const { return m_isFinished; }

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

    void update(sf::Time _dt);

private:
    struct BoardParticipant
    {
        Participant& participantRef;
        StepId currentStepId = StepId::Idle;
    };

    BoardState getNextState(BoardState _state) const;
    void enterState(BoardState _state);
    void updateState(BoardState _state, sf::Time _dt);
    void processInputs();

    BoardParticipant& getBoardParticipant(PlayerId _id);
    void participantStartsTurn(BoardParticipant& _participant);
    void onParticipantTimeout(BoardParticipant& _participant);

    void setParticipantStep(StepId _stepId, BoardParticipant& _participantRef);
    size_t getIndexOfNextParticipantTurn(size_t _currentIndex) const;
    void processInputEvent(const events::RemotePlayerInputNetEvent& _event);

    void processDecideCardStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant);
    void processDrawCardStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant);
    void processFinishTurnStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant);
    void processMatchCardStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant);
    void processSeeCardStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant);
    void processSwapCardStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant);
    void processTakeCardStep(const events::RemotePlayerInputNetEvent& _event, BoardParticipant& _participant);

    void discardCard(Card* _card);
    Card* getNextCardFromDeck();

    const core::Context& m_contextRef;
    core::event::ListenerId m_listenerId{};

    Deck& m_deckRef;
    Discard& m_discardRef;
    std::vector<BoardParticipant> m_participants;

    size_t m_indexOfCurrentParticipantTurn = 0;
    bool m_currentParticipantFinishedTurn = false;

    BoardState m_state = BoardState::Start;
    bool m_newStateRequested = false;
    sf::Time m_timeoutDt = {};

    std::map<int32_t, events::RemotePlayerInputNetEvent> m_inputBuffer;

    size_t m_playersLeftBeforeEnd = 0;

    Card* m_drawnCardPtr = nullptr;
    ClickSlotInputData m_swapData;
    bool m_isFirstPartOfSwap = true;

    bool m_isFinished = false;
};

} // namespace cn::server::game
