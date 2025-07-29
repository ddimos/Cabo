#pragma once

#include "shared/board/Participant.hpp"
#include "shared/game/Object.hpp"

namespace cn::shared::game
{

class Participant : public Object
{
public:
    Participant(board::Participant& _boardParticipant)
        : Object(_boardParticipant) {}

    board::Participant& getBoardParticipant() const { return static_cast<board::Participant&>(getBoardObject()); }
};

} // namespace cn::shared::game
