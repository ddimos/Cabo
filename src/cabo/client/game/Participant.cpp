#include "client/game/Participant.hpp"
#include "shared/game/Constants.hpp"

namespace cn::client::game
{

Participant::Participant(const core::Context& _context, shared::game::object::Id _id, PlayerId _playerId, bool _isLocal)
    : shared::game::object::Participant(_id, _playerId)
    , m_isLocal(_isLocal)
    , m_interpolatorPos(shared::game::MoveUpdateDuration, core::Easing::linear)
{
}

void Participant::setMousePosition(sf::Vector2f _pos)
{
    if (m_isLocal)
        shared::game::object::Participant::setMousePosition(_pos);
    else
        m_interpolatorPos.start(_pos);
}

void Participant::onUpdate(sf::Time) 
{
    if (m_isLocal)
        return;

    auto pos = sf::Vector2f(m_interpolatorPos.get());
    shared::game::object::Participant::setMousePosition(pos);
}

} // namespace cn::client::game
