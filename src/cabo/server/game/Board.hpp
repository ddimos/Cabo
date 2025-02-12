#pragma once

#include "core/Context.hpp"
#include "server/game/Deck.hpp"
#include "server/game/Participant.hpp"
#include "server/game/Types.hpp"

#include <vector>

namespace cn::server::game
{

class Board final
{
public:
    Board(const core::Context& _context, Deck* _deck, std::vector<Participant*>&& _participants);
    ~Board();

    Deck* getDeck() const { return m_deck; }

    const core::Context& getContext() const { return m_contextRef; };

private:
    const core::Context& m_contextRef;

    Deck* m_deck;
    std::vector<Participant*> m_participants;
};

} // namespace cn::server::game
