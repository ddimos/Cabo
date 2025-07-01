#pragma once

#include "core/object/Object.hpp"
#include "shared/board/Deck.hpp"

namespace cn::client::game
{

class Deck final : public core::object::Object
{
public:
    Deck(shared::board::Deck& _boardDeck) : m_boardDeckRef(_boardDeck) {}

    shared::board::Deck& getBoardDeck() { return m_boardDeckRef; }

private:
    shared::board::Deck& m_boardDeckRef;
};

} // namespace cn::client::game
