#pragma once

#include "server/game/Deck.hpp"
#include "server/game/Participant.hpp"
#include "server/game/Step.hpp"
#include "server/game/Types.hpp"

#include "core/event/Types.hpp"
#include "core/Context.hpp"

#include <vector>

namespace cn::server::game
{

class Board final
{
public:
    Board(const core::Context& _context, Deck& _deck, std::vector<Participant*>&& _participants);
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
        std::unique_ptr<Step> currentStep;
    };
    BoardParticipant& getBoardParticipant(PlayerId _id);
    void setParticipantStep(StepId _stepId, BoardParticipant& _participantRef);

    const core::Context& m_contextRef;

    Deck& m_deckRef;
    std::vector<BoardParticipant> m_participants;
    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;

    BoardState m_state = BoardState::Start;
    bool m_newStateRequested = false;
};

} // namespace cn::server::game
