#pragma once

#include "shared/board/PrivateZone.hpp"
#include "shared/game/Object.hpp"

namespace cn::shared::game
{

class PrivateZone : public Object
{
public:
    PrivateZone(board::PrivateZone& _boardPrivateZone)
        : Object(_boardPrivateZone)
    {}

    board::PrivateZone& getBoardPrivateZone() const { return static_cast<board::PrivateZone&>(getBoardObject()); }
};

} // namespace cn::shared::game
