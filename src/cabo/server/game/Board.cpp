#include "server/game/Board.hpp"

namespace cn::server::game
{

Board::Board(const core::Context& _context, Deck* _deck, std::vector<Participant*>&& _participants)
    : m_contextRef(_context)
    , m_deck(_deck)
    , m_participants(std::move(_participants))
{
}

Board::~Board()
{
}

} // namespace cn::server::game
