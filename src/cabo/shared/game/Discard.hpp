#pragma once

#include "shared/board/Discard.hpp"
#include "shared/game/Object.hpp"

namespace cn::shared::game
{

class Discard : public shared::game::Object
{
public:
    Discard(shared::board::Discard& _boardDiscard) : m_boardDiscardRef(_boardDiscard) {}

    shared::board::Object& getBoardObject() const { return m_boardDiscardRef; }
    shared::board::Discard& getBoardDiscard() const { return m_boardDiscardRef; }

protected:
    shared::board::Discard& m_boardDiscardRef;
};

} // namespace cn::shared::game
