#pragma once

#include "shared/board/Participant.hpp"
#include "shared/game/Object.hpp"

namespace cn::shared::game
{

class Participant : public shared::game::Object
{
public:
    Participant(shared::board::Participant& _boardParticipant) : m_boardParticipantRef(_boardParticipant) {}

    shared::board::Object& getBoardObject() const { return m_boardParticipantRef; }
    shared::board::Participant& getBoardParticipant() const { return m_boardParticipantRef; }

protected:
    shared::board::Participant& m_boardParticipantRef;
};

} // namespace cn::shared::game
