#pragma once

#include "shared/board/BoardObject.hpp"

namespace cn::shared::board
{
    
class Card : public BoardObject
{
public:
    Card(BoardObjectId _id);
};

} // namespace cn::shared::board
