#include "client/game/Board.hpp"
#include "client/game/step/SeeOwnCard.hpp"

namespace cn::client::game
{

Board::Board(const core::Context& _context, std::vector<game::Participant*>&& _participants)
    : m_contextRef(_context)
    , m_participants(std::move(_participants))

{

}

void Board::registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{

}

void Board::update(sf::Time _dt)
{

}

void Board::start()
{
    CN_ASSERT(!hasGameStarted());

  
}

bool Board::hasGameStarted() const
{
    return true;//m_playingState != State::None;
}

void Board::onLocalPlayerClickDeck()
{
    if (!hasGameStarted())
        return;

}

} // namespace cn::client::game
