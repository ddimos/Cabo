#pragma once

#include "shared/board/Card.hpp"
#include "shared/game/Object.hpp"

namespace cn::shared::game
{

class Card : public shared::game::Object
{
public:
    Card(shared::board::Card& _boardCard) : m_boardCardRef(_boardCard) {}

    shared::board::Object& getBoardObject() const { return m_boardCardRef; }
    shared::board::Card& getBoardCard() const { return m_boardCardRef; }

protected:
    shared::board::Card& m_boardCardRef;
};

} // namespace cn::shared::game
