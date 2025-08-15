#include "client/game/Participant.hpp"
#include "shared/game/Constants.hpp"

namespace cn::client::game
{

Participant::Participant(const core::Context& _context, shared::board::Participant& _boardParticipant, bool _isLocal)
    : shared::game::Participant(_boardParticipant)
    , m_isLocal(_isLocal)
    , m_interpolatorPos(_context.get<sf::Clock>(), shared::game::MoveUpdateDuration, core::Easing::linear)
{
}

void Participant::addPosition(sf::Vector2f _pos)
{
    m_interpolatorPos.start(_pos);
}

void Participant::onUpdate(sf::Time) 
{
    if (m_isLocal)
        return;

    auto pos = sf::Vector2f(m_interpolatorPos.get());
    getBoardParticipant().setMousePosition(pos);
}

} // namespace cn::client::game
