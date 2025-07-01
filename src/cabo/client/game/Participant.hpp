#pragma once

#include "core/object/Object.hpp"
#include "shared/board/Participant.hpp"

namespace cn::client::game
{

class Participant final : public core::object::Object
{
public:
    Participant(shared::board::Participant& _boardParticipant) : m_boardParticipantRef(_boardParticipant) {}
    
    shared::board::Participant& getBoardParticipant() { return m_boardParticipantRef; }

private:
    shared::board::Participant& m_boardParticipantRef;
};

} // namespace cn::client::game
