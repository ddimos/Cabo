#pragma once

#include "core/object/Object.hpp"
#include "shared/board/Discard.hpp"

namespace cn::client::game
{

class Discard final : public core::object::Object
{
public:
    Discard(shared::board::Discard& _boardDiscard) : m_boardDiscardRef(_boardDiscard) {}
    
    shared::board::Discard& getBoardDiscard() { return m_boardDiscardRef; }

private:
    shared::board::Discard& m_boardDiscardRef;
};

} // namespace cn::client::game
