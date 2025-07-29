#pragma once

#include "shared/board/Anchor.hpp"
#include "shared/game/Object.hpp"

namespace cn::shared::game
{

class Anchor : public Object
{
public:
    Anchor(board::Anchor& _boardAnchor)
        : Object(_boardAnchor)
    {}

    board::Anchor& getBoardAnchor() const { return static_cast<board::Anchor&>(getBoardObject()); }
};

} // namespace cn::shared::game
