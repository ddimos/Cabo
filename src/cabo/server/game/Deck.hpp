#pragma once

#include "core/object/Object.hpp"
#include "shared/board/Deck.hpp"

namespace cn::server::game
{

class Deck final : public core::object::Object
{
public:
    Deck(shared::board::Deck& _boardDeck) : m_boardDeckRef(_boardDeck) {}

    shared::board::Deck& m_boardDeckRef;
};

} // namespace cn::server::game
