#pragma once

#include "shared/board/Discard.hpp"
#include "shared/game/Object.hpp"

namespace cn::shared::game
{

class Discard : public Object
{
public:
    Discard(board::Discard& _boardDiscard)
        : Object(_boardDiscard)
    {}

    board::Discard& getBoardDiscard() const { return static_cast<board::Discard&>(getBoardObject()); }
};

} // namespace cn::shared::game
