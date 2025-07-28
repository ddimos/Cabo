#pragma once

#include "shared/board/Anchor.hpp"
#include "shared/game/Object.hpp"

namespace cn::shared::game
{

class Anchor : public shared::game::Object
{
public:
    Anchor(shared::board::Anchor& _boardAnchor) : m_boardAnchorRef(_boardAnchor) {}

    shared::board::Object& getBoardObject() const { return m_boardAnchorRef; }
    shared::board::Anchor& getBoardAnchor() const { return m_boardAnchorRef; }

protected:
    shared::board::Anchor& m_boardAnchorRef;
};

} // namespace cn::shared::game
